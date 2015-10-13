//
// Created by yexiaokang on 2015/10/13.
//

#define CPP_COMPILE

#include "util/char.h"
#include "util/cplusplus/socket.cpp"

#define NELEM(x)  (sizeof(x) / sizeof((x)[0]))


static const char *classPath = "com/example/fine/ndksample/ndkInterface/HttpUtil";

/**
 * java class static method use jclass, none-static method use jobject
 */
static jstring doPostRequest(JNIEnv *env, jclass clazz, jstring host, jint port, jstring url, jstring content, jint flag) {
    LOGD("doPostRequest!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env);
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
static jstring doGetRequest(JNIEnv *env, jclass clazz, jstring host, jint port, jstring url, jint flag) {
    LOGD("doGetRequest!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env);
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
static jstring doSocketConnect(JNIEnv *env, jclass clazz, jstring host, jint port, jstring content, jint flag) {
    LOGD("doSocketConnect!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env);
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

static JNINativeMethod methods[] = {
        {"doPostRequest",   "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;", (void *) doPostRequest},
        {"doGetRequest",    "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;",                   (void *) doGetRequest},
        {"doSocketConnect", "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;",                   (void *) doSocketConnect}
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv Failed !");
        return JNI_ERR;
    }

    jclass clazz = env->FindClass(classPath);
    if (clazz == NULL) {
        LOGE("FindClass Failed !");
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, methods, NELEM(methods)) < 0) {
        LOGE("RegisterNatives Failed !");
        return JNI_ERR;
    }
    LOGI("registerNatives success !");

    return JNI_VERSION_1_4;
}