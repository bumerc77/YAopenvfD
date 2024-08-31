LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
	
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/bin

LOCAL_SRC_FILES := \
	src/main.c \
	src/openvfd.c \
	src/collector.c \
	src/cli.c \
	src/dots.c \
	src/glyphs.c \
	src/reporter.c \
	src/watcher.c \
	src/version.c \
	src/collector/string.c \
	src/collector/temp.c \
	src/collector/io.c \
	src/collector/cpu.c\
	src/collector/net.c \
	src/collector/date.c \
	src/collector/size.c
	
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_CFLAGS += -DBINARY=YAopenvfD

ifeq ($(DEBUG_VFD),true)
VERSION := 1.1-4a54ca4-20240611
LOCAL_CFLAGS += -DVERSION=\"$(VERSION)\"
else
LOCAL_CFLAGS += -DVERSION=unknown
endif
	
LOCAL_CFLAGS := -Wall -Wextra -Wno-unused-function	
LOCAL_MODULE := YAopenvfD
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_EXECUTABLE)
