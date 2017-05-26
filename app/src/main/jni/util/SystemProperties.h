//
// Created by yexiaokang on 2017/5/26.
//

#ifndef _SYSTEM_PROPERTIES_H
#define _SYSTEM_PROPERTIES_H

#include "Log.h"
#include <dlfcn.h>
#include <stdio.h>

typedef int (*PFN_SYSTEM_PROP_GET)(const char *, char *);

// test on Android 7.0, it works well
int __system_property_get2(const char *name, char *value) {
    static PFN_SYSTEM_PROP_GET __real_system_property_get = NULL;
    if (!__real_system_property_get) {
        // libc.so should already be open, get a handle to it.
        void *handle = dlopen("libc.so", RTLD_NOW);
        if (!handle) {
            ALOGE("Cannot dlopen libc.so: %s.\n", dlerror());
        } else {
            __real_system_property_get = (PFN_SYSTEM_PROP_GET) dlsym(handle, "__system_property_get");
        }
        if (!__real_system_property_get) {
            ALOGE("Cannot resolve __system_property_get(): %s.\n", dlerror());
            return 0;
        }
    }
    return (*__real_system_property_get)(name, value);
}

#endif //_SYSTEM_PROPERTIES_H
