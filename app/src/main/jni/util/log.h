//
// Created by yexiaokang on 2015/9/2.
//

#ifndef JNISOCKET_LOG_H
#define JNISOCKET_LOG_H

#include <jni.h>
#include <android/log.h>

#define DEBUG 1
#define LINUX 1
#define BUFFER_SIZE 1024
#define RECV_SIZE BUFFER_SIZE * 2
#define PACKET_SIZE BUFFER_SIZE * 8

#define LOG_TAG "android_native"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)


#endif //JNISOCKET_LOG_H