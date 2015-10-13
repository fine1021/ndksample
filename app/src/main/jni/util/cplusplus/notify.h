//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_NOTIFY_CPLUSPLUS_H
#define NDKSAMPLE_NOTIFY_CPLUSPLUS_H

/**
 * call java class method
 * jni String Type : java/lang/String  or  Ljava/lang/String;
 * javap -s xx.class query parameter type
 */
void notifyMessage(JNIEnv *env, jobject obj, const char *msg) {
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

    jstring msgString = env->NewStringUTF(msg);
    // void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    env->CallVoidMethod(obj, method, msgString);
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
void notifyMessageObj(JNIEnv *env, const char *msg) {
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

#endif //NDKSAMPLE_NOTIFY_CPLUSPLUS_H
