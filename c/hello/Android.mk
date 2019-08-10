LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	hello.c

LOCAL_SHARED_LIBRARIES := \
	liblog

LOCAL_CFLAGS := -Wno-unused-parameter

LOCAL_MODULE_TAGS := samples

LOCAL_MODULE := hello_elf_arm64


include $(BUILD_EXECUTABLE)
