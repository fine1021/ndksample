//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_SOCKET_H
#define NDKSAMPLE_SOCKET_H

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "../log.h"
#include <stdio.h>
#include <stdlib.h>
#include "notify.h"

#ifdef ANDROID

#include <netinet/in.h>      // sockaddr_in struct
#include <sys/types.h>       // basic system data types
#include <sys/socket.h>      // sockaddr struct
#include <sys/time.h>        // timeval for select
#include <unistd.h>          // close socket
#include <string.h>          // bzero
#include <arpa/inet.h>       // inet_addr htons inet_ntoa inet_aton
#include <netdb.h>           // hostent
#include <errno.h>           // errno

#endif

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")                    // link with Ws2_32.lib
#include <WinSock2.h>
#include <WS2tcpip.h>                                 // win IPV6 net_ntop inet_pton
#include <windows.h>                                  // after WinSock2.h
#define snprintf _snprintf                            // linux C snprintf
//#define snprintf sprintf_s
#define bzero(src, c) memset(src, 0, c)               // linux C bzero
#define bcopy(src, dest, c) memcpy(dest, src, c)      // linux C bcopy
#define close(fd) closesocket(fd)                     // linux C close
#define usleep(dwMilliseconds) Sleep(dwMilliseconds)  // linux C usleep
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
    char socketHost[128];            // domain name
    char socketIP[128];              // ip of the domain name
    SOCKET_ADDR socket_addr;
    struct timeval select_timeout;
    fd_set rset;
    char logMsg[PACKET_SIZE];        // log message
    bool isConvert;                  // if need to convert domain name to ip
    JNIEnv *env;                     // jni call java method

    void init();

    void callJavaMethod();

    Status getHostByName(char *host);

    int formatString(char *str, int flag);

public:
    SocketHelper();

    ~SocketHelper();

    Status initEnv(JNIEnv *env);

    Status setConvert(bool value);

    Status createSocket(char *host, int port);

    Status connectSocket();

    Status sendMessage(char *content);

    Status sendHttpPostMsg(char *url, char *content);

    Status sendHttpGetMsg(char *url);

    Status recvMessage(char *buffer, int bufferSize);

    Status closeSocket();
};

#endif //NDKSAMPLE_SOCKET_H
