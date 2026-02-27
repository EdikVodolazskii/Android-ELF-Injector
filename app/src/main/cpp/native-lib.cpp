#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "AgentExercise"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_agentexercise_Utility_loadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);
    LOGI("Method: loadLib, Parameter: %s", path);
    env->ReleaseStringUTFChars(libPath, path);
}

JNIEXPORT void JNICALL
Java_com_example_agentexercise_Utility_unloadLib(JNIEnv *env, jobject thiz, jstring libPath) {
    const char *path = env->GetStringUTFChars(libPath, nullptr);
    LOGI("Method: unloadLib, Parameter: %s", path);
    env->ReleaseStringUTFChars(libPath, path);
}
}