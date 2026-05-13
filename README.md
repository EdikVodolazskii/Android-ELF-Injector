# Android Dynamic Loading and ELF Patching

This repository contains a comprehensive implementation of several technical assignments focused on Android native code execution, runtime dynamic library loading, and programmatic manipulation of ELF binary structures using the LIEF framework and Frida Gadget.

The solutions are isolated into specific branches to facilitate a modular review of each assignment's requirements.

## Repository Structure (Branch Navigation)

* **`main`** — **Exercises 1 and 2**: Foundational integration of native C++ code, JNI bridging, and the use of native library constructors.
* **`exercise_3`** — **Exercises 3 and 4**: Implementation of a custom runtime resource extractor and a dynamic loader using POSIX APIs.
* **`exercise_5`** (and **`exercise_6`**) — **Exercises 5 and 6**: Integration of the LIEF framework and programmatic analysis of ELF dynamic dependencies.
* **`exercise_7`** — **Exercise 7**: Advanced dynamic ELF patching, dependency injection, and bypassing Android Linker Namespace restrictions.

---

## Technical Implementation and Architectural Decisions

The following sections detail the technical rationale behind the chosen implementation strategies, based on an analysis of Android's internal security architecture and binary structures.

### 1. Automated Execution via Native Constructors (Exercises 1 and 2)
To ensure the native library (`libhello.so`) executes logic immediately upon being mapped into the process memory, the `__attribute__((constructor))` directive was utilized in C++. 
* **Rationale:** This approach ensures the execution of `onLibLoad()` is handled by the dynamic linker itself at the moment of loading, providing a reliable trigger for logging and initialization that does not depend on explicit calls from the Java layer.

### 2. Custom Loader and Professional Error Handling (Exercises 3 and 4)
Standard Android library loading via `System.loadLibrary()` is restricted to verified paths within the APK. To simulate the behavior of a dynamic plugin system or an unpacker, a manual extraction and loading routine was developed.
* **Implementation:** Libraries are stored as raw resources, extracted to `getFilesDir()`, and loaded via a custom native wrapper (`libdynamicloader.so`).
* **Technical Detail (`dlerror`):** The implementation utilizes the `dlopen` API with the `RTLD_NOW` flag. Crucially, professional-grade error handling was integrated by capturing the output of `dlerror()` upon failure. This allowed for the identification of specific issues such as architecture mismatches or missing dependencies, which are otherwise obscured by generic `UnsatisfiedLinkError` exceptions in Java.
* **Security Compliance:** Because Android’s SELinux policy frequently restricts execution from data directories, the Java layer explicitly manages file permissions using `setExecutable(true, false)` and `setReadable(true, false)` to ensure the loader has sufficient privileges to execute the extracted binaries.

### 3. LIEF Integration via CMake (Exercises 5 and 6)
The **LIEF** (Library to Instrument Executable Formats) framework was selected for binary manipulation. 
* **Implementation:** LIEF was integrated directly into the project's build system using the `FetchContent` module in `CMakeLists.txt`. It is statically linked (`libLIEF.a`) into the native loader.
* **Rationale:** Static linking was chosen to ensure the final application is self-contained. This eliminates the complexity of managing LIEF as a separate dynamic dependency on the target device and allows for robust ELF parsing of the `.dynamic` section to identify `DT_NEEDED` entries directly on the mobile device.

### 4. Dynamic ELF Patching for Instrumentation (Exercise 7)
The objective was to force `libhello.so` to load `libgadget.so` (Frida Gadget) automatically. 
* **Implementation:** Manual hex-editing of ELF files is prone to offset errors. The `patchLib` JNI method uses LIEF to programmatically insert a new `DT_NEEDED` entry into the dynamic table. LIEF handles the recalculation of string table offsets and Program Headers, ensuring the resulting binary remains valid and loadable by the Android system.

### 5. Bypassing Linker Namespace Restrictions (Exercise 7)
A significant challenge in modern Android (7.0+) is the **Linker Namespace** mechanism, which prevents the system linker from resolving dependencies found in the application's local data folders.
* **The Problem:** Even if `libhello.so` is patched to require `libgadget.so`, a standard load call will fail because the linker is not permitted to search the local directory for the new dependency.
* **The Solution (Pre-loading):** To bypass this, the application implements a pre-loading sequence. The loader first explicitly calls `dlopen` on the absolute path of `libgadget.so`. This places the gadget in the process's global memory map. When the subsequent load of the patched `libhello.so` occurs, the linker finds the dependency already resident in memory and successfully completes the linkage, effectively circumventing the namespace restriction.

---

## Testing Instructions for Exercise 7

To verify the successful injection and execution of the instrumentation payload, follow these steps:

### Step 1: Host Machine Preparation
Ensure your development environment is configured to communicate with the Frida Gadget:

1. **Install Frida Tools:**
   ```bash
   pip install frida-tools
   ```
2. **Configure Port Forwarding:**
   Frida Gadget listens on port `27042` by default. Forward this port from the device to your host:
   ```bash
   adb forward tcp:27042 tcp:27042
   ```

### Step 2: Payload and Environment Setup
1. Switch to the `exercise_7` branch, build, and install the application.
2. Create a local file named `script.js` with the following test payload:
   ```javascript
   Java.perform(function() {
       var Log = Java.use('android.util.Log');
       Log.d('AgentExercise', 'GREAT JOB !');
   });
   ```

### Step 3: Execution
1. Launch the application and tap **Patch**. This invokes LIEF to modify the binary on the device.
2. Tap **Load**. 
   * The application will execute the pre-loading logic.
   * The UI will enter a suspended state (freeze). This is the expected behavior of Frida Gadget in **Listen** mode.
   * Logcat will display: `Listening on 127.0.0.1 TCP port 27042`.
3. From your host terminal, inject the script:
   ```bash
   frida -U -N com.example.agentexercise -l script.js
   ```

**Expected Result:**

```log
FridaLog                com.example.agentexercise            D  GREAT JOB !
```

Monitor the Android Logcat for the tag `AgentExercise`. The appearance of the message **`GREAT JOB !`** confirms that:

1. The ELF binary was successfully patched.
2. The Linker Namespace restriction was bypassed via pre-loading.
3. The Frida Gadget successfully initialized and executed the JavaScript payload within the ART Virtual Machine.
4. Frida is ready to work and execute scripts from the console line:

```console
     ____
    / _  |   Frida 17.7.3 - A world-class dynamic instrumentation toolkit
   | (_| |
    > _  |   Commands:
   /_/ |_|       help      -> Displays the help system
   . . . .       object?   -> Display information about 'object'
   . . . .       exit/quit -> Exit
   . . . .
   . . . .   More info at [https://frida.re/docs/home/](https://frida.re/docs/home/)
   . . . .
   . . . .   Connected to 127.0.0.1:27042 (id=socket@127.0.0.1:27042)

[Remote::Gadget ]-> Java.perform(function() { console.log("Phone model: " + Java.use("android.os.Build").MODEL.value); })
Phone model: sdk_gphone64_x86_64
```
