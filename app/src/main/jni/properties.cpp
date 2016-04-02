//
// Created by yexiaokang on 2016/4/1.
//

#include <cutils/properties.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/JNIHelp.h"

const char *className = "com/example/fine/ndksample/SystemProperties";


jstring SystemProperties_native_get(JNIEnv *env, jobject clazz, jstring keyJ, jstring defJ) {

    int len;
    const char *key;
    char buf[PROPERTY_VALUE_MAX];
    jstring rvJ = NULL;

    if (keyJ == NULL) {
        LOGE("key is null");
        return rvJ;
    }

    key = env->GetStringUTFChars(keyJ, NULL);

    len = property_get(key, buf, "");

    if ((len <= 0) && (defJ != NULL)) {
        rvJ = defJ;
    } else if (len >= 0) {
        rvJ = env->NewStringUTF(buf);
    } else {
        rvJ = env->NewStringUTF("");
    }

    LOGI("key = %s, value = %s", key, buf);

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


