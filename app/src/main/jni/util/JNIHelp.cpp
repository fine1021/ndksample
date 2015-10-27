//
// Created by fine on 2015/10/27.
//

#include "JNIHelp.h"
#include "reference.h"

int jniRegisterNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *gMethods, int numMethods) {

    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("FindClass Failed !");
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives Failed !");
        return JNI_ERR;
    }
    LOGI("RegisterNatives success !");

    return JNI_OK;
}