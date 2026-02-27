#include <android/log.h>

#define LOG_TAG "AgentExercise"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

__attribute__((constructor))
void onLibLoad() {
    LOGI("libhello was loaded");
}