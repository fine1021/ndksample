#include "util/char.h"
#include "util/c/socket.h"
#include "util/c/notify.h"
#include "com_example_fine_ndksample_ndkInterface_HttpUtil.h"


/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doPostRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doPostRequest(JNIEnv *env, jclass jcls, jstring host, jint port,
                                                                            jstring url, jstring content, jint transform) {
    return (*env)->NewStringUTF(env, "Hello from JNI ! ");
}

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doGetRequest
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doGetRequest(JNIEnv *env, jclass jcls, jstring host, jint port,
                                                                           jstring url, jint transform) {
    return (*env)->NewStringUTF(env, "Hello from JNI ! ");
}

/*
 * Class:     com_example_fine_ndksample_ndkInterface_HttpUtil
 * Method:    doSocketConnect
 * Signature: (Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;
 */
jstring Java_com_example_fine_ndksample_ndkInterface_HttpUtil_doSocketConnect(JNIEnv *env, jclass jcls, jstring host, jint port,
                                                                              jstring content, jint transform) {
	LOGD("doSocketConnect!");
	const char *ip = (*env)->GetStringUTFChars(env, host, NULL);
    notifyMessage(env,"initSocket");
    initSocket(ConstCharToChar(ip), port);
    notifyMessage(env, "connectSocket");
	connectSocket();
    notifyMessage(env,"sendMessage");
	sendMessage("hello");
	char msg[PACKET_SIZE] = {0};
    notifyMessage(env,"recvMessage");
	recvMessage(msg);
    notifyMessage(env,"closeSocket");
    closeSocket();
    (*env)->ReleaseStringUTFChars(env, host, ip);
    return (*env)->NewStringUTF(env, msg);
}
