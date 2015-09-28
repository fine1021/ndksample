//
// Created by yexiaokang on 2015/9/2.
//

#ifndef JNISOCKET_COMMON_H
#define JNISOCKET_COMMON_H

#include <jni.h>
#include <android/log.h>

#define DEBUG

#define LOG_TAG "NativeMethod"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)

/**
 * call java class method
 * jni String Type : java/lang/String  or  Ljava/lang/String;
 * javap -s xx.class query parameter type
 */
void notifyMessage(JNIEnv *env, jobject obj, const char *msg) {
    jclass cls = (*env)->FindClass(env, "com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }
    LOGI("find class");

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = (*env)->GetMethodID(env, cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }
    LOGI("find method ");

    // void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    (*env)->CallVoidMethod(env, obj, method, (*env)->NewStringUTF(env, msg));
}

/**
 * call java class static method
 */
void notifyMessageStatic(JNIEnv *env, jobject obj, char *msg) {
    jclass cls = (*env)->FindClass(env, "com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }
    LOGI("find class");
    // jmethodID (*GetStaticMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = (*env)->GetStaticMethodID(env, cls, "notifyMessage",
                                                 "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }
    LOGI("find method ");

    // void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    (*env)->CallStaticVoidMethod(env, obj, method, (*env)->NewStringUTF(env, msg));
}

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyMessageObj(JNIEnv *env, jobject obj, const char *msg) {
    jclass cls = (*env)->FindClass(env, "com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }
    LOGI("find class");

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    // java/lang/String Ljava/lang/String;
    jmethodID method = (*env)->GetMethodID(env, cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }
    LOGI("find method ");

    //  void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //  jobject (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    //  jobject (*AllocObject)(JNIEnv*, jclass);

    jobject jobj = (*env)->AllocObject(env, cls);

    (*env)->CallVoidMethod(env, jobj, method, (*env)->NewStringUTF(env, msg));
}


#endif //JNISOCKET_COMMON_H

/*
jobject     (*CallObjectMethod)(JNIEnv*, jobject, jmethodID, ...);
jobject     (*CallObjectMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jobject     (*CallObjectMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jboolean    (*CallBooleanMethod)(JNIEnv*, jobject, jmethodID, ...);
jboolean    (*CallBooleanMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jboolean    (*CallBooleanMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jbyte       (*CallByteMethod)(JNIEnv*, jobject, jmethodID, ...);
jbyte       (*CallByteMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jbyte       (*CallByteMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jchar       (*CallCharMethod)(JNIEnv*, jobject, jmethodID, ...);
jchar       (*CallCharMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jchar       (*CallCharMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jshort      (*CallShortMethod)(JNIEnv*, jobject, jmethodID, ...);
jshort      (*CallShortMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jshort      (*CallShortMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jint        (*CallIntMethod)(JNIEnv*, jobject, jmethodID, ...);
jint        (*CallIntMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jint        (*CallIntMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jlong       (*CallLongMethod)(JNIEnv*, jobject, jmethodID, ...);
jlong       (*CallLongMethodV)(JNIEnv*, jobject, jmethodID, va_list);
jlong       (*CallLongMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
jfloat      (*CallFloatMethod)(JNIEnv*, jobject, jmethodID, ...) __NDK_FPABI__;
jfloat      (*CallFloatMethodV)(JNIEnv*, jobject, jmethodID, va_list) __NDK_FPABI__;
jfloat      (*CallFloatMethodA)(JNIEnv*, jobject, jmethodID, jvalue*) __NDK_FPABI__;
jdouble     (*CallDoubleMethod)(JNIEnv*, jobject, jmethodID, ...) __NDK_FPABI__;
jdouble     (*CallDoubleMethodV)(JNIEnv*, jobject, jmethodID, va_list) __NDK_FPABI__;
jdouble     (*CallDoubleMethodA)(JNIEnv*, jobject, jmethodID, jvalue*) __NDK_FPABI__;
void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
void        (*CallVoidMethodV)(JNIEnv*, jobject, jmethodID, va_list);
void        (*CallVoidMethodA)(JNIEnv*, jobject, jmethodID, jvalue*); */