
#define LOG_TAG "ledNative"
#include <log/log.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jni.h"
#include <hardware/hardware.h>
#include <led_hal.h>

static const char *classPathName = "com/example/lowlevel/LedNative";
static led_module_t* pModule = NULL;
static led_device_t* pDevice = NULL;

jint openLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    jint ret = 0;
    ALOGD("------%s", __FUNCTION__);

    ret = hw_get_module(LED_HARDWARE_MODULE_ID, (const struct hw_module_t **)&pModule);
    if (ret != 0) {
        ALOGE("get hardware module '%s' failed", LED_HARDWARE_MODULE_ID);
        return -1;
    }
    /* pModule->common.methods->open((const struct hw_module_t *)pModule, NULL, (struct hw_device_t**)&pDevice); */
    led_hal_open((const struct hw_module_t*)pModule, &pDevice);
    if (pDevice == NULL) {
        ALOGE("open hardware device failed");
        return -1;
    }

    return 0;
}

jint closeLed(JNIEnv* /*env*/, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);
    if (pDevice == NULL) {
        ALOGE("hardware device is NULL");
        return -1;
    }
    /* pDevice->common.close((struct hw_device_t*)pDevice);*/
    led_hal_close(pDevice);

    return 0;
}


jint ledOn(JNIEnv* /*env*/, jobject /*thiz*/)
{
    int ret;
    ALOGD("------%s", __FUNCTION__);

    if (pDevice == NULL) {
        ALOGE("hardware device is NULL");
        return -1;
    }
    ret = pDevice->control(1);

    return ret;
}

jint ledOff(JNIEnv* /*env*/, jobject /*thiz*/)
{
    int ret;
    ALOGD("------%s", __FUNCTION__);

    if (pDevice == NULL) {
        ALOGE("hardware device is NULL");
        return -1;
    }
    ret = pDevice->control(0);

    return ret;
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

