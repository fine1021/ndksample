//
// Created by yexiaokang on 2015/10/15.
//

#ifndef NDKSAMPLE_INTERFACE_H
#define NDKSAMPLE_INTERFACE_H

#include "../util/JNIHelp.h"
#include "../util/reference.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {

    void (*initJavaVM)(JavaVM *vm);                             // init javaVM

    void (*setCallback)(jobject obj);                           // setCallback

    int (*notifyMessage)(const char *msg);                      // specify a class

    int (*notifyMessageStatic)(const char *msg);                // static method

    int (*notifyMessageWithObj)(const char *msg);               // custom callBack Obj class

} JavaMethodInterface;

JavaMethodInterface *getJavaMethodInterface();

#ifdef __cplusplus
}
#endif

#endif //NDKSAMPLE_INTERFACE_H
