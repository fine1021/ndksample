//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_NOTIFY_H
#define NDKSAMPLE_NOTIFY_H

#include "../JNIHelp.h"

/**
 * call java class method
 * jni String Type : java/lang/String  or  Ljava/lang/String;
 * javap -s xx.class query parameter type
 */
void notifyMessageWithObj(JNIEnv *env, jobject obj, const char *msg) {

    jclass cls = (*env)->GetObjectClass(env, obj);
    if (cls == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = (*env)->GetMethodID(env, cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    // void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    (*env)->CallVoidMethod(env, obj, method, (*env)->NewStringUTF(env, msg));
}

/**
 * call java class static method
 */
void notifyMessageStatic(JNIEnv *env, char *msg) {

    jclass cls = (*env)->FindClass(env, "com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }
    // jmethodID (*GetStaticMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = (*env)->GetStaticMethodID(env, cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    // void (*CallStaticVoidMethod)(JNIEnv*, jclass, jmethodID, ...);
    (*env)->CallStaticVoidMethod(env, cls, method, (*env)->NewStringUTF(env, msg));
}

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyMessage(JNIEnv *env, const char *msg) {

    jclass cls = (*env)->FindClass(env, "com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    // java/lang/String Ljava/lang/String;
    jmethodID method = (*env)->GetMethodID(env, cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    //  void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //  jobject (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    //  jobject (*AllocObject)(JNIEnv*, jclass);

    jobject obj = (*env)->AllocObject(env, cls);

    (*env)->CallVoidMethod(env, obj, method, (*env)->NewStringUTF(env, msg));
}


#endif //NDKSAMPLE_NOTIFY_H
