//
// Created by yexiaokang on 2015/9/2.
//

#ifndef JNISOCKET_LOG_H
#define JNISOCKET_LOG_H

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define RECV_SIZE BUFFER_SIZE * 2
#define PACKET_SIZE BUFFER_SIZE * 8

#define FLAG_BASE 1                     // int(4 bytes) have 32 bit, use a bit to enable a function, we can enable several functions at the same time
#define FLAG_CONVERT FLAG_BASE
#define FLAG_NOT_CONVERT FLAG_BASE << 1

#define LOG_TAG "android_native"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)


#endif //JNISOCKET_LOG_H