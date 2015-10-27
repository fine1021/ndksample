//
// Created by yexiaokang on 2015/9/2.
//

#ifndef NDKSAMPLE_REFERENCE_H
#define NDKSAMPLE_REFERENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define RECV_SIZE BUFFER_SIZE * 2
#define PACKET_SIZE BUFFER_SIZE * 8

#define FLAG_BASE 1                     // int(4 bytes) have 32 bit, use a bit to enable a function, we can enable several functions at the same time
#define FLAG_CONVERT FLAG_BASE
#define FLAG_NOT_CONVERT FLAG_BASE << 1


#endif //NDKSAMPLE_REFERENCE_H