//
// Created by CHENGUANG491 on 2018/1/23.
//

#ifndef PACHEN_COOL_ANDROID_LOG_PRINT_H
#define PACHEN_COOL_ANDROID_LOG_PRINT_H

#include <android/log.h>

#define IS_DEBUG

#ifdef IS_DEBUG

#define LOG_TAG_PA ("PaChenJni")

#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG_PA, __VA_ARGS__))

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG_PA, __VA_ARGS__))

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO   , LOG_TAG_PA, __VA_ARGS__))

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN   , LOG_TAG_PA, __VA_ARGS__))

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR  , LOG_TAG_PA, __VA_ARGS__))

#else

#define LOGV(LOG_TAG_PA, ...) NULL

#define LOGD(LOG_TAG_PA, ...) NULL

#define LOGI(LOG_TAG_PA, ...) NULL

#define LOGW(LOG_TAG_PA, ...) NULL

#define LOGE(LOG_TAG_PA, ...) NULL

#endif

#endif
