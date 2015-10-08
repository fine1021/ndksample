//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_SOCKET_H
#define NDKSAMPLE_SOCKET_H

#include "../log.h"
#include <stdio.h>
#include <stdlib.h>
#include "notify.h"

#ifdef LINUX

#include <netinet/in.h>      // sockaddr_in struct
#include <sys/types.h>       // basic system data types
#include <sys/socket.h>      // sockaddr struct
#include <sys/time.h>        // timeval for select
#include <unistd.h>          // close socket
#include <string.h>          // bzero
#include <arpa/inet.h>       // inet_addr htons inet_ntoa inet_aton
#include <netdb.h>           // hostent

#endif

#ifdef WIN32
#include <windows.h>
#define snprintf _snprintf                          // linux C snprintf
//#define snprintf sprintf_s
#define bzero(src, c) memset(src, 0, c)             // linux C bzero
#define bcopy(src, dest, c) memcpy(dest, src, c)    // linux C bcopy
#endif

#define OK 1
#define ERROR 0
typedef int Status;

typedef struct SOCKET_ADDR {
    union {
        struct sockaddr_in addr4;     // IPV4
        struct sockaddr_in6 addr6;    // IPV6
    };
} SOCKET_ADDR;

class SocketHelper {

private:
    int socketfd;
    char address[128];           // IP address
    SOCKET_ADDR socket_addr;
    struct timeval select_timeout;
    fd_set rset;
    char logMsg[PACKET_SIZE];    // log message
    bool isTransform;            // if need to transform host to ip
    bool isDebug;                // debug mode

    /* ADD : jni call java method */
    JNIEnv *env;
    jobject obj;

    void init();

    void log();

    void callJavaMethod();

    Status getHostByName(char *host);

public:
    SocketHelper();

    ~SocketHelper();

    Status initEnv(JNIEnv *env, jobject obj);

    Status setTransform(bool value);

    Status setDebug(bool value);

    Status createSocket(char *host, int port);

    Status connectSocket();

    Status sendMessage(char *buffer);

    Status sendHttpPostMsg(char *buffer);

    Status sendHttpGetMsg(char *buffer);

    Status recvMessage(char *buffer);

    Status closeSocket();
};

#endif //NDKSAMPLE_SOCKET_H
