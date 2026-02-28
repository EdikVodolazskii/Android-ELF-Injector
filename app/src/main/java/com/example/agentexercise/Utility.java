package com.example.agentexercise;

public class Utility {
    static {
        System.loadLibrary("dynamicloader");
    }
    public native void loadLib(String libPath);

    public native void unloadLib(String libPath);

    public native void patchLib(String libPath);
}
