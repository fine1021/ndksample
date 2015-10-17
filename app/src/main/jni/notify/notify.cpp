//
// Created by yexiaokang on 2015/10/15.
//

#include "notify.h"

/**
 * call java class method
 * jni String Type : java/lang/String  or  Ljava/lang/String;
 * javap -s xx.class query parameter type
 */
void notifyMessageWithObj(JNIEnv *env, const char *msg) {

    if(callbackObject == NULL) {
        LOGE("callbackObject == NULL");
        return;
    }

    jclass clazz = env->GetObjectClass(callbackObject);
    if (clazz == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetMethodID(clazz, "notifyToast", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    jstring msgString = env->NewStringUTF(msg);
    // void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    env->CallVoidMethod(callbackObject, method, msgString);
    env->DeleteLocalRef(msgString);
}

/**
 * call java class static method
 */
void notifyMessageStatic(JNIEnv *env, char *msg) {

    jclass clazz = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (clazz == 0) {
        LOGE("find class error");
        return;
    }
    // jmethodID (*GetStaticMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetStaticMethodID(clazz, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    jstring msgString = env->NewStringUTF(msg);
    // void (*CallStaticVoidMethod)(JNIEnv*, jclass, jmethodID, ...);
    env->CallStaticVoidMethod(clazz, method, msgString);
    env->DeleteLocalRef(msgString);
}

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyMessage(JNIEnv *env, const char *msg) {

    jclass clazz = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (clazz == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetMethodID(clazz, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    //  void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //  jobject (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    //  jobject (*AllocObject)(JNIEnv*, jclass);

    jstring msgString = env->NewStringUTF(msg);
    jobject obj = env->AllocObject(clazz);
    env->CallVoidMethod(obj, method, msgString);
    env->DeleteLocalRef(msgString);
}

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyToast(JNIEnv *env, const char *msg) {

    jclass clazz = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (clazz == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetMethodID(clazz, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    //  void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //  jobject (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    //  jobject (*AllocObject)(JNIEnv*, jclass);

    jstring msgString = env->NewStringUTF(msg);
    jobject obj = env->AllocObject(clazz);
    env->CallVoidMethod(obj, method, msgString);
    env->DeleteLocalRef(msgString);
}

void initJavaVM(JavaVM *vm) {
    jvm = vm;
}

void javaMethodCallback(jobject obj) {
    callbackObject = obj;
}

int callJavaMethod(char *msg) {
    if (jvm != NULL) {
        JNIEnv *env = NULL;
        if (jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("GetEnv Failed !");
            return JNI_ERR;
        }
        notifyMessage(env, msg);
    }
}

int callJavaMethodStatic(char *msg) {
    if (jvm != NULL) {
        JNIEnv *env = NULL;
        if (jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("GetEnv Failed !");
            return JNI_ERR;
        }
        notifyMessageStatic(env, msg);
    }
}

int callJavaMethodWithObj(char *msg) {
    if (jvm != NULL) {
        JNIEnv *env = NULL;
        if (jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("GetEnv Failed !");
            return JNI_ERR;
        }
        notifyMessageWithObj(env, msg);
    }
}

JavaMethodInterface javaMethodInterface = {
        initJavaVM,
        javaMethodCallback,
        callJavaMethod,
        callJavaMethodStatic,
        callJavaMethodWithObj
};

JavaMethodInterface *getJavaMethodInterface() {
    return &javaMethodInterface;
}
