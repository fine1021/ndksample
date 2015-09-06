//
// Created by yexiaokang on 2015/9/2.
//

#ifndef JNISOCKET_COMMON_H
#define JNISOCKET_COMMON_H

#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#define LOG_TAG "NativeMethod"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)


#endif //JNISOCKET_COMMON_H
