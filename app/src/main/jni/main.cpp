//
// Created by yexiaokang on 2015/9/29.
//

#include "util/char.h"
#include "socket/socket.cpp"
#include "com_example_fine_ndksample_ndkInterface_HttpUtil.h"

JavaVM *javaVM;

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doPostRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doPostRequest(JNIEnv *env, jclass clazz, jstring host, jint port,
                                                                            jstring url, jstring content, jint flag) {
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
/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doGetRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doGetRequest(JNIEnv *env, jclass clazz, jstring host, jint port,
                                                                           jstring url, jint flag) {
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

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass clazz, jstring host, jint port,
                                                                              jstring content, jint flag) {
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

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv Failed !");
        return JNI_ERR;
    }
    LOGI("JNI_OnLoad success !");

    return JNI_VERSION_1_4;
}
