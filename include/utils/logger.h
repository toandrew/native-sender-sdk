#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdarg.h>

void print_message(const char *format,...);

#ifndef ANDROID_LOG
#define LOGD(format,args...)	print_message(format,##args)
#define LOGI(format,args...) 	print_message(format,##args)
#define LOGW(format,args...) 	print_message(format,##args)
#define LOGE(format,args...) 	print_message(format,##args)
#else
#define LOG_TAG         	    "native_fling_sdk"
#include <utils/Log.h>
#define LOGD					ALOGD
#define LOGI					ALOGI
#define LOGW 					ALOGW
#define LOGE 					ALOGE
#endif // ANDROID_LOG

#endif // LOG_H_
