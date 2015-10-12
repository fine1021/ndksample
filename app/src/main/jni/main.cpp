//
// Created by yexiaokang on 2015/9/29.
//
#define CPP_COMPILE

#include "util/char.h"
#include "util/cplusplus/socket.cpp"
#include "com_example_fine_ndksample_ndkInterface_HttpUtil.h"

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doPostRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doPostRequest(JNIEnv *env, jclass obj, jstring host, jint port,
                                                                                              jstring url, jstring content, jint transform) {
    LOGD("doPostRequest!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env, obj);
    socketHelper.setTransform(transform == 1);
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
/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doGetRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doGetRequest(JNIEnv *env, jclass obj, jstring host, jint port,
                                                                                             jstring url, jint transform) {
    LOGD("doGetRequest!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env,obj);
    socketHelper.setTransform(transform == 1);
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

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass obj, jstring host, jint port,
                                                                                                jstring content, jint transform) {
    LOGD("doSocketConnect!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env,obj);
    socketHelper.setTransform(transform == 1);
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
