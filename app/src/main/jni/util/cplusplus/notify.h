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
    jclass cls = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetMethodID(cls, "notifyMessage", "(Ljava/lang/String;)V");
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
void notifyMessageStatic(JNIEnv *env, jobject obj, char *msg) {
    jclass cls = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }
    // jmethodID (*GetStaticMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetStaticMethodID(cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    jstring msgString = env->NewStringUTF(msg);
    // void (*CallStaticVoidMethod)(JNIEnv*, jclass, jmethodID, ...);
    env->CallStaticVoidMethod(cls, method, msgString);
    env->DeleteLocalRef(msgString);
}

/**
 * call java class method
 * the java class which calls jni native methods, is not the class which has callbacks
 */
void notifyMessageObj(JNIEnv *env, jobject obj, const char *msg) {
    jclass cls = env->FindClass("com/example/fine/ndksample/ndkInterface/Messenger");
    if (cls == 0) {
        LOGE("find class error");
        return;
    }

    // jmethodID (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
    jmethodID method = env->GetMethodID(cls, "notifyMessage", "(Ljava/lang/String;)V");
    if (method == 0) {
        LOGE("find method error");
        return;
    }

    //  void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    //  jobject (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    //  jobject (*AllocObject)(JNIEnv*, jclass);

    jstring msgString = env->NewStringUTF(msg);
    jobject object = env->AllocObject(cls);
    env->CallVoidMethod(object, method, msgString);
    env->DeleteLocalRef(msgString);
}

#endif //NDKSAMPLE_NOTIFY_CPLUSPLUS_H
