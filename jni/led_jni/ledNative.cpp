
#define LOG_TAG "ledNative.cpp"
#include <android/log.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "jni.h"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static const char *classPathName = "com/example/lowlevel/LedNative";
static const char *ledDevNode = "/sys/devices/platform/leds/leds/user_led3/brightness";
static int fd;

jint openLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);

    fd = open(ledDevNode, O_RDWR);
    if (fd < 0) {
        ALOGE("open: %s", strerror(errno));
        return -1;
    }

    return 0;
}

jint closeLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);

    close(fd);
    return 0;
}


jint ledOn(JNIEnv* /*env*/, jobject /*thiz*/)
{
    jint ret;
    ALOGD("------%s, fd=%d", __FUNCTION__, fd);

    ret = write(fd, "255", 4);
    if (ret < 0) {
        ALOGE("write: %s", strerror(errno));
        return -1;
    }
    return 0;
}

jint ledOff(JNIEnv* /*env*/, jobject /*thiz*/)
{
    jint ret;
    ALOGD("------%s, fd=%d", __FUNCTION__, fd);

    ret = write(fd, "0", 2);
    if (ret < 0) {
        ALOGE("write: %s", strerror(errno));
        return -1;
    }
    return 0;
}

static JNINativeMethod ledMethod[] = {
  {"openDev", "()I", (void*)openLed},
  {"closeDev", "()I", (void*)closeLed},
  {"devOn", "()I", (void*)ledOn},
  {"devOff", "()I", (void*)ledOff},
};

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    jint ret;
    JNIEnv* env = NULL;

    ALOGD("------%s", __FUNCTION__);
    ret = vm->GetEnv((void**)&env, JNI_VERSION_1_4);
    if (ret != 0) {
        ALOGE("vm->GetEnv error, ret = %d", ret);
        return -1;
    }

    jclass cls = env->FindClass(classPathName);
    if (cls == NULL) {
        ALOGE("Native registration unable to find class '%s'", classPathName);
        return JNI_FALSE;
    }

    ret = env->RegisterNatives(cls, ledMethod, sizeof(ledMethod)/sizeof(ledMethod[0]));
    if (ret < 0) {
        ALOGE("RegisterNatives failed for '%s'", classPathName);
        return JNI_FALSE;
    }

    return JNI_VERSION_1_4;
}

