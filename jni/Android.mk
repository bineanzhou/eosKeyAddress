# 
#


LOCAL_PATH := $(call my-dir)



#############################################################################
## SO for App
#############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := lib_eos_maester_shr
LOCAL_LDLIBS:=-llog
include $(LOCAL_PATH)/objects_lib.mk

LOCAL_CFLAGS := -static -Wimplicit-function-declaration
include $(BUILD_SHARED_LIBRARY)
#############################################################################



#############################################################################
## LIB for App
#############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := lib_eos_maester
include $(LOCAL_PATH)/objects_lib.mk

LOCAL_CFLAGS := -static -Wimplicit-function-declaration
include $(BUILD_STATIC_LIBRARY)
#############################################################################

