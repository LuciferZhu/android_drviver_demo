
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

static const char *classPathName = "com/example/lowlevel/LedNativePlus";
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

jint add_num(JNIEnv* /*env*/, jobject /*thiz*/, jint a, jint b)
{
    ALOGD("------%s, a=%d,b=%d", __FUNCTION__, a, b);
    return a + b;
}

jstring get_jni_string(JNIEnv* env, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);
    return env->NewStringUTF("This is from JNI.");
}

void set_jni_string(JNIEnv* env, jobject /*thiz*/, jstring str)
{
    ALOGD("------%s", __FUNCTION__);
    const char* tmp;
    tmp = env->GetStringUTFChars(str, NULL);
    ALOGD("get string from java:'%s'", tmp);
    env->ReleaseStringUTFChars(str, tmp);
}

jobject get_jni_student(JNIEnv* env, jobject /*thiz*/)
{
    ALOGD("------%s", __FUNCTION__);
    jclass stucls = env->FindClass("com/example/ledcontrolplus/Student");

    jfieldID nameID = env->GetFieldID(stucls, "name", "Ljava/lang/String;");
    jfieldID ageID = env->GetFieldID(stucls, "age", "I");
    jfieldID heightID = env->GetFieldID(stucls, "height", "D");
    jmethodID setAllID = env->GetMethodID(stucls, "setAll", "(IDLjava/lang/String;)V");
    /* get construst methodID */
    jmethodID constructID = env->GetMethodID(stucls, "<init>", "()V");

    /* new a student object */
    jobject stuobj = env->NewObject(stucls, constructID);

    /* init all field */
    env->SetIntField(stuobj, ageID, 26);
    env->SetDoubleField(stuobj, heightID, 1.78);
    env->SetObjectField(stuobj, nameID, env->NewStringUTF("soengtung Tshu"));

    /* demo call a java method */
    env->CallVoidMethod(stuobj, setAllID, 34, 1.98, env->NewStringUTF("Kobe"));

    return stuobj;
}

void set_jni_student(JNIEnv* env, jobject /*thiz*/, jobject stuobj)
{
    ALOGD("------%s", __FUNCTION__);
    jclass stucls = env->GetObjectClass(stuobj);

    jfieldID nameID = env->GetFieldID(stucls, "name", "Ljava/lang/String;");
    jfieldID ageID = env->GetFieldID(stucls, "age", "I");
    jfieldID heightID = env->GetFieldID(stucls, "height", "D");

    jint age = env->GetIntField(stuobj, ageID);
    jdouble height = env->GetDoubleField(stuobj, heightID);
    jstring nameStr = (jstring)env->GetObjectField(stuobj, nameID);

    const char* name = env->GetStringUTFChars(nameStr, NULL);
    ALOGI("get Student obj from java,\n"
            "age = %d, height = %lf, name = %s",
            age, height, name);
    env->ReleaseStringUTFChars(nameStr, name);
}

static JNINativeMethod ledMethod[] = {
  {"openDev", "()I", (void*)openLed},
  {"closeDev", "()I", (void*)closeLed},
  {"devOn", "()I", (void*)ledOn},
  {"devOff", "()I", (void*)ledOff},

  {"addNum", "(II)I", (void*)add_num},
  {"getStringFromJNI", "()Ljava/lang/String;", (void*)get_jni_string},
  {"setStringToJNI", "(Ljava/lang/String;)V", (void*)set_jni_string},
  {"getStudentFromJNI", "()Lcom/example/ledcontrolplus/Student;", (void*)get_jni_student},
  {"setStudentToJNI", "(Lcom/example/ledcontrolplus/Student;)V", (void*)set_jni_student},
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

