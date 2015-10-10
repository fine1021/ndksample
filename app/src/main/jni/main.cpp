//
// Created by yexiaokang on 2015/9/29.
//
#define CPP_COMPILE

#include "util/char.h"
#include "util/cplusplus/socket.cpp"
#include "com_example_fine_ndksample_ndkInterface_HttpUtil.h"

/*
 * Class:     com_yxkang_jnisocket_HttpUtil
 * Method:    doPostRequest
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doPostRequest(JNIEnv *env, jclass obj, jstring string, jint i) {
    return string;
}

/*
 * Class:     com_yxkang_jnisocket_HttpUtil
 * Method:    doGetRequest
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doGetRequest(JNIEnv *env, jclass obj, jstring string, jint i) {
    return env->NewStringUTF("Hello from JNI ! ");
}

/*
 * Class:     com_yxkang_jnisocket_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass obj, jstring host, jint port) {
    LOGD("doSocketConnect!");
    SocketHelper socketHelper;
    socketHelper.initEnv(env,obj);
    const char *temp = env->GetStringUTFChars(host, NULL);
    socketHelper.createSocket(ConstCharToChar(temp), port);
    socketHelper.connectSocket();
    socketHelper.sendHttpPostMsg(ConstCharToChar(temp));
    //socketHelper.sendHttpGetMsg(ConstCharToChar(temp));
    //socketHelper.sendMessage(StringToChar("hello"));
    char msg[PACKET_SIZE] = {0};
    socketHelper.recvMessage(msg);
    socketHelper.closeSocket();
    env->ReleaseStringUTFChars(host, temp);
    return env->NewStringUTF(msg);
}
