//
// Created by yexiaokang on 2016/4/1.
//
#define LOG_NDEBUG 0
#define LOG_TAG "SystemProperties"

#include "util/JNIHelp.h"
#include <sys/system_properties.h>

const char *className = "com/example/fine/ndksample/SystemProperties";


jstring SystemProperties_native_get(JNIEnv *env, jobject clazz, jstring keyJ, jstring defJ) {

    int len;
    const char *key;
    char buf[PROP_VALUE_MAX];
    jstring rvJ = NULL;

    if (keyJ == NULL) {
        LOGE("key is null");
        return rvJ;
    }

    key = env->GetStringUTFChars(keyJ, NULL);

    len = __system_property_get(key, buf);

    if ((len <= 0) && (defJ != NULL)) {
        rvJ = defJ;
    } else if (len >= 0) {
        rvJ = env->NewStringUTF(buf);
    } else {
        rvJ = env->NewStringUTF("");
    }

    LOGI("__system_property_get key = %s, value = %s", key, buf);

    env->ReleaseStringUTFChars(keyJ, key);

    return rvJ;
}


static const JNINativeMethod sMethods[] = {
        {"native_get", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", (void *) SystemProperties_native_get}
};

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }
    LOGI("Retrieve the env success !");

    jniRegisterNativeMethods(env, className, sMethods, NELEM(sMethods));

    return JNI_VERSION_1_4;
}


