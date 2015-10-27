//
// Created by yexiaokang on 2015/10/13.
//

#include "util/char.h"
#include "util/JNIHelp.h"
#include "socket/socket.cpp"

JavaVM *javaVM;
jobject callbackObj;
const char *className = "com/example/fine/ndksample/ndkInterface/HttpUtil";

namespace android {

    /**
     * java class static method use jclass, none-static method use jobject
     */
    jstring doPostRequest(JNIEnv *env, jclass clazz, jstring host, jint port, jstring url, jstring content, jint flag) {
        LOGD("doPostRequest!");
        SocketHelper socketHelper;
        socketHelper.setConvert(flag & FLAG_CONVERT);
        const char *postHost = env->GetStringUTFChars(host, NULL);
        socketHelper.createSocket(ConstCharToChar(postHost), port);
        socketHelper.connectSocket();
        const char *postUrl = env->GetStringUTFChars(url, NULL);
        const char *postContent = env->GetStringUTFChars(content, NULL);
        socketHelper.sendHttpPostMsg(ConstCharToChar(postUrl), ConstCharToChar(postContent));
        char msg[PACKET_SIZE] = {0};
        socketHelper.recvMessage(msg, PACKET_SIZE);
        socketHelper.closeSocket();
        env->ReleaseStringUTFChars(host, postHost);
        env->ReleaseStringUTFChars(url, postUrl);
        env->ReleaseStringUTFChars(content, postContent);
        return env->NewStringUTF(msg);
    }

    /**
     * java class static method use jclass, none-static method use jobject
     */
    jstring doGetRequest(JNIEnv *env, jclass clazz, jstring host, jint port, jstring url, jint flag) {
        LOGD("doGetRequest!");
        SocketHelper socketHelper;
        socketHelper.setConvert(flag & FLAG_CONVERT);
        const char *getHost = env->GetStringUTFChars(host, NULL);
        socketHelper.createSocket(ConstCharToChar(getHost), port);
        socketHelper.connectSocket();
        const char *getUrl = env->GetStringUTFChars(url, NULL);
        socketHelper.sendHttpGetMsg(ConstCharToChar(getUrl));
        char msg[PACKET_SIZE] = {0};
        socketHelper.recvMessage(msg, PACKET_SIZE);
        socketHelper.closeSocket();
        env->ReleaseStringUTFChars(host, getHost);
        env->ReleaseStringUTFChars(url, getUrl);
        return env->NewStringUTF(msg);
    }

    /**
     * java class static method use jclass, none-static method use jobject
     */
    jstring doSocketConnect(JNIEnv *env, jclass clazz, jstring host, jint port, jstring content, jint flag) {
        LOGD("doSocketConnect!");
        SocketHelper socketHelper;
        socketHelper.setConvert(flag & FLAG_CONVERT);
        const char *sendHost = env->GetStringUTFChars(host, NULL);
        socketHelper.createSocket(ConstCharToChar(sendHost), port);
        socketHelper.connectSocket();
        const char *sendContent = env->GetStringUTFChars(content, NULL);
        socketHelper.sendMessage(ConstCharToChar(sendContent));
        char msg[PACKET_SIZE] = {0};
        socketHelper.recvMessage(msg, PACKET_SIZE);
        socketHelper.closeSocket();
        env->ReleaseStringUTFChars(host, sendHost);
        env->ReleaseStringUTFChars(content, sendContent);
        return env->NewStringUTF(msg);
    }

    /*
     * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
     * Method:    setCallback
     * Signature: (Ljava/lang/Object;)V
     */
    void setCallback(JNIEnv *env, jclass clazz, jobject obj) {
        callbackObj = env->NewGlobalRef(obj);
    }

    static const JNINativeMethod sMethods[] = {
            {"doPostRequest",   "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;", (void *) doPostRequest},
            {"doGetRequest",    "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;",                   (void *) doGetRequest},
            {"doSocketConnect", "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;",                   (void *) doSocketConnect},
            {"setCallback",     "(Ljava/lang/Object;)V",                                                        (void *) setCallback}
    };

    int register_ndksample_ndkInterface_HttpUtil(JNIEnv * env, JavaVM * vm) {
        javaVM = vm;
        return jniRegisterNativeMethods(env, className, sMethods, NELEM(sMethods));
    }

}