//
// Created by yexiaokang on 2017/5/25.
//

#ifndef _LOG_H
#define _LOG_H

#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------

/*
 * Normally we strip ALOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define LOG_NDEBUG 0"
 * at the top of your source file) to change that behavior.
 */
#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

/*
 * This is the local tag used for the following simplified
 * logging macros.  You can change this preprocessor definition
 * before using the other macros to change the tag.
 */
#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

// ---------------------------------------------------------------------

#ifndef ALOG
#define ALOG(...) __android_log_print(__VA_ARGS__)
#endif

/*
 * Simplified macro to send a verbose log message using the current LOG_TAG.
 */
#ifndef ALOGV
#define __ALOGV(...) ((void)ALOG(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#if LOG_NDEBUG
#define ALOGV(...) do { if (0) { __ALOGV(__VA_ARGS__); } } while (0)
#else
#define ALOGV(...) __ALOGV(__VA_ARGS__)
#endif
#endif

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#ifndef ALOGV_IF
#if LOG_NDEBUG
#define ALOGV_IF(cond, ...)   ((void)0)
#else
#define ALOGV_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif
#endif

/*
 * Simplified macro to send a debug log message using the current LOG_TAG.
 */
#ifndef ALOGD
#define ALOGD(...) ((void)ALOG(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGD_IF
#define ALOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an info log message using the current LOG_TAG.
 */
#ifndef ALOGI
#define ALOGI(...) ((void)ALOG(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGI_IF
#define ALOGI_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send a warning log message using the current LOG_TAG.
 */
#ifndef ALOGW
#define ALOGW(...) ((void)ALOG(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGW_IF
#define ALOGW_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

/*
 * Simplified macro to send an error log message using the current LOG_TAG.
 */
#ifndef ALOGE
#define ALOGE(...) ((void)ALOG(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

#ifndef ALOGE_IF
#define ALOGE_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )
#endif

// ---------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //_LOG_H
