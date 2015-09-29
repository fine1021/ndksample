//
// Created by yexiaokang on 2015/9/29.
//
#define CPP_COMPILE

#include "util/char.h"
#include "util/cplusplus/socket.cpp"
#include "util/cplusplus/notify.h"
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
JNIEXPORT jstring JNICALL JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass obj, jstring ip, jint port) {
    LOGD("doSocketConnect!");
    SocketHelper socketHelper;
    const char *temp = env->GetStringUTFChars(ip, NULL);
    notifyMessageObj(env, obj, StringToChar("initSocket"));
    socketHelper.createSocket(ConstCharToChar(temp), port);
    env->ReleaseStringUTFChars(ip, temp);
    notifyMessageObj(env, obj, StringToChar("connectSocket"));
    socketHelper.connectSocket();
    notifyMessageObj(env, obj, StringToChar("sendMessage"));
    socketHelper.sendMessage(StringToChar("hello"));
    char msg[BUFFER_SIZE] = {0};
    notifyMessageObj(env, obj, StringToChar("recvMessage"));
    socketHelper.recvMessage(msg);
    notifyMessageObj(env, obj, StringToChar("closeSocket"));
    socketHelper.closeSocket();
    return env->NewStringUTF("Hello from JNI !");
}
