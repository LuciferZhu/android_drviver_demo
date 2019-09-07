
#define LOG_TAG "ledNative.cpp"
#include <log/log.h>
#include <hidl/HidlSupport.h>
#include <hidl/Status.h>
#include <my_demo/hardware/led/1.0/ILed.h>
#include "jni.h"

using my_demo::hardware::led::V1_0::ILed;
using android::sp;
using android::status_t;

static const char *classPathName = "com/example/lowlevel/LedNative";

jint openLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);

    return 0;
}

jint closeLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);

    return 0;
}


jint ledOn(JNIEnv* /*env*/, jobject /*thiz*/)
{
    jint ret;
    ALOGD("------%s", __FUNCTION__);

    sp<ILed> hidlService = ILed::getService();
    if (hidlService == nullptr) {
        ALOGE("can't not get ILed service");
        return -1;
    }

    ret = hidlService->control(1);
    if(ret < 0) {
        ALOGE("hidlService->control(1) failed");
        return -1;
    }
    return 0;
}

jint ledOff(JNIEnv* /*env*/, jobject /*thiz*/)
{
    jint ret;
    ALOGD("------%s", __FUNCTION__);

    sp<ILed> hidlService = ILed::getService();
    if (hidlService == nullptr) {
        ALOGE("can't not get ILed service");
        return -1;
    }

    ret = hidlService->control(0);
    if(ret < 0) {
        ALOGE("hidlService->control(0) failed");
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

