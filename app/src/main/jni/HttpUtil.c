#include "com_example_fine_ndksample_ndkInterface_HttpUtil.h"
#include "SocketUtil.h"
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
    return string;
}

/*
 * Class:     com_yxkang_jnisocket_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL JNICALL Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass obj, jstring string, jint i) {

	LOGD("doSocketConnect!");
	const char *ip = (*env)->GetStringUTFChars(env, string, NULL);
	char IP[100];
	strncpy(IP, ip, sizeof(IP));
	initSocket(IP, 80);
	connectSocket();
	sendMessage("hello");
	char msg[BUFFER_SIZE] = {0};
	recvMessage(msg);
    closeSocket();
    return string;
}
