LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS := libsupc++:libs/libsupc++.a libcurl:libs/libcurl.a
include $(BUILD_MULTI_PREBUILT)
include $(CLEAR_VARS)

LOCAL_MODULE:= flint_sdk_test

LOCAL_SRC_FILES := \
        src/main.cpp \
        src/service/ssdp_service.cpp \
        src/ssdp/device_scanner.cpp \
        src/ssdp/ssdp.cpp \
        src/ssdp/ssdp_device_scanner.cpp \
        src/ssdp/ssdp_search_msg.cpp \
        src/ssdp/ssdp_socket.cpp \
        src/ssdp/utils/datagram_packet.cpp \
        src/ssdp/utils/datagram_socket.cpp \
        src/ssdp/utils/idevice_scan_listener.cpp \
        src/ssdp/utils/inet_address.cpp \
        src/ssdp/utils/inet_socket_address.cpp \
        src/ssdp/utils/multicast_socket.cpp \
        src/ssdp/utils/network_interface.cpp \
        src/ssdp/utils/parsed_datagram.cpp \
        src/ssdp/utils/pattern.cpp \
        src/ssdp/utils/runnable.cpp \
        src/ssdp/utils/socket_address.cpp \
        src/ssdp/utils/thread.cpp \
        src/ssdp/utils/timer.cpp \
        src/utils/logger.cpp \
        src/http/http_client.cpp \
		src/tinyxml2/tinyxml2.cpp \
		src/flint/flint_device.cpp \

#LOCAL_CPPFLAGS += -Wno-format -DHAVE_CONFIG_H  -D_THREAD_SAFE -DDEBUG -O0 -g
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DANDROID -DEXPAT_RELATIVE_PATH -DALLOW_QUOTED_COOKIE_VALUES -DCOMPONENT_BUILD -DGURL_DLL -DANDROID_NDK
LOCAL_CPPFLAGS += -fexceptions  -fno-rtti -DANDROID_NDK

LOCAL_C_INCLUDES := \
	frameworks/base/include \
	ndk/sources/cxx_stl/gnu-libstdc++/4.6/libs/armeabi-v7a/include \
	ndk/sources/cxx_stl/gnu-libstdc++/4.6/include \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/service/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ssdp/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ssdp/utils/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/utils/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/http/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/http/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/tinyxml2/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/flint/

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libstlport \
	libz


LOCAL_STATIC_LIBRARIES := libsupc++ libcurl

LOCAL_MODULE_TAGS := option

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)

#include $(BUILD_SHARED_LIBRARY)
include external/stlport/libstlport.mk
include $(BUILD_EXECUTABLE)
