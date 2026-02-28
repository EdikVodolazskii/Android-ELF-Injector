#include <jni.h>
#include <string>
#include <android/log.h>
#include <dlfcn.h>
#include <LIEF/LIEF.hpp>
#include "LIEF/version.h"

#define LOG_TAG "AgentExercise"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void* libHandle = nullptr;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_agentexercise_Utility_patchLib(JNIEnv *env, jobject thiz, jstring lib_path)
{
    const char *path = env->GetStringUTFChars(lib_path, nullptr);

    std::unique_ptr<LIEF::ELF::Binary> binary = LIEF::ELF::Parser::parse(path);

    if (binary == nullptr)
    {
        LOGI("LIEF error log!!!");
    }
    else{
        LOGI("The binary was parsed successfully!");
    }

    if (binary && !binary->has_library("libgadget.so")) {
        binary->add_library("libgadget.so");
        binary->write(path);

        LOGI("LIEF patched and saved libhello.so!");
    }

    env->ReleaseStringUTFChars(lib_path, path);
}

JNIEXPORT void JNICALL
Java_com_example_agentexercise_Utility_loadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    LOGI("LIEF SDK Version: %s", LIEF_VERSION);

    const char *path = env->GetStringUTFChars(libPath, nullptr);
    LOGI("Method: loadLib, Path: %s", path);

    libHandle = dlopen(path, RTLD_NOW);

    if (libHandle != nullptr) {
        LOGI("dlopen SUCCESS");
    } else {
        LOGI("dlopen FAILED: %s", dlerror());
    }

    env->ReleaseStringUTFChars(libPath, path);
}

JNIEXPORT void JNICALL
Java_com_example_agentexercise_Utility_unloadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);
    LOGI("Method: unloadLib, Path: %s", path);

    if (libHandle != nullptr) {
        int result = dlclose(libHandle);
        if (result == 0) {
            LOGI("dlclose SUCCESS");
            libHandle = nullptr;
        } else {
            LOGI("dlclose FAILED: %s", dlerror());
        }
    } else {
        LOGI("Library is not loaded yet!");
    }

    env->ReleaseStringUTFChars(libPath, path);
}
}
