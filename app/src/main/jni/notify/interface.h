//
// Created by yexiaokang on 2015/10/15.
//

#ifndef NDKSAMPLE_INTERFACE_H
#define NDKSAMPLE_INTERFACE_H

#include "../util/log.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {

    void (*initJavaVM)(JavaVM *vm);                             // init javaVM

    int (*notifyMessage)(char *msg);                            // specify a class

    int (*notifyMessageStatic)(char *msg);                      // static method

    int (*notifyMessageWithCallback)(jobject obj, char *msg);   // custom class

} JavaMethodInterface;

JavaMethodInterface *getJavaMethodInterface();

#ifdef __cplusplus
}
#endif

#endif //NDKSAMPLE_INTERFACE_H
