//
// Created by yexiaokang on 2015/10/27.
//

#include "util/JNIHelp.h"
#include "util/reference.h"

namespace android {

    int register_ndksample_ndkInterface_HttpUtil(JNIEnv * env, JavaVM * vm);
};

using namespace android;

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("GetEnv failed!");
        return result;
    }
    LOGI("Retrieve the env success !");

    register_ndksample_ndkInterface_HttpUtil(env, vm);

    return JNI_VERSION_1_4;
}