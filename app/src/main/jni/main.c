#include "util/char.h"
#include "util/c/socket.h"
#include "util/c/notify.h"
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
    return (*env)->NewStringUTF(env, "Hello from JNI ! ");
}

/*
 * Class:     com_yxkang_jnisocket_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass obj, jstring string, jint i) {
	LOGD("doSocketConnect!");
	const char *ip = (*env)->GetStringUTFChars(env, string, NULL);
    notifyMessageObj(env, obj,"initSocket");
    initSocket(ConstCharToChar(ip), i);
    notifyMessageObj(env, obj, "connectSocket");
	connectSocket();
    notifyMessageObj(env, obj,"sendMessage");
	sendMessage("hello");
	char msg[PACKET_SIZE] = {0};
    notifyMessageObj(env, obj,"recvMessage");
	recvMessage(msg);
    notifyMessageObj(env, obj,"closeSocket");
    closeSocket();
    (*env)->ReleaseStringUTFChars(env, string, ip);
    return string;
}
