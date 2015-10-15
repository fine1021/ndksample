# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lm
LOCAL_MODULE := socket
LOCAL_SRC_FILES := main2.cpp
LOCAL_CFLAGS := -DANDROID -DCPP_COMPILE

include $(BUILD_SHARED_LIBRARY)

# second library

include $(CLEAR_VARS)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lm
LOCAL_MODULE := notify
LOCAL_SRC_FILES := notify/notify.cpp
LOCAL_CFLAGS := -DANDROID -DCPP_COMPILE

include $(BUILD_SHARED_LIBRARY)

# third library

include $(CLEAR_VARS)

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lm
LOCAL_MODULE := socket2
LOCAL_SRC_FILES := main.c
LOCAL_CFLAGS := -DANDROID

include $(BUILD_SHARED_LIBRARY)