//
// Created by fine on 2015/10/27.
//

#ifndef NDKSAMPLE_JNIHELP_H
#define NDKSAMPLE_JNIHELP_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Normally we strip LOGV (VERBOSE messages) from release builds.
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

#ifndef LOGV
#if LOG_NDEBUG
#define LOGV(...)   ((void)0)
#else
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#endif
#endif

#ifndef LOGD
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGI
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGW
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef NELEM
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

int jniRegisterNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *gMethods, int numMethods);

#ifdef __cplusplus
}
#endif

#endif //JNIHELP_H
