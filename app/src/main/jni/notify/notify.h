//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_NOTIFY_CPLUSPLUS_H
#define NDKSAMPLE_NOTIFY_CPLUSPLUS_H

#include "interface.h"

JavaVM *jvm = NULL;
jobject callbackObject;

/**
 * call java class method
 * jni String Type : java/lang/String  or  Ljava/lang/String;
 * javap -s xx.class query parameter type
 */
void notifyMessageWithObj(JNIEnv *env, const char *msg);

/**
 * call java class static method
 */
void notifyMessageStatic(JNIEnv *env, char *msg);

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyMessage(JNIEnv *env, const char *msg);

#endif //NDKSAMPLE_NOTIFY_CPLUSPLUS_H
