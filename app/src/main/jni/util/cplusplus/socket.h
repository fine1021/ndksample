//
// Created by yexiaokang on 2015/9/29.
//

#ifndef NDKSAMPLE_SOCKET_H
#define NDKSAMPLE_SOCKET_H

#include "../log.h"

#ifdef POSIX /* START POSIX */

#include <netinet/in.h>      // sockaddr_in struct
#include <sys/types.h>       // socket data type
#include <sys/socket.h>      // sockaddr struct
#include <sys/time.h>        // timeval
#include <unistd.h>          // close socket
#include <string.h>          // bzero
#include <arpa/inet.h>       // inet_addr htons inet_ntoa inet_aton

#endif /* END POSIX */

#ifdef WIN32
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
        struct sockaddr_in addr4; // IPV4
        struct sockaddr_in6 addr6; // IPV6
    };
} SOCKET_ADDR;

class SocketHelper {

private:
    int socketfd;
    SOCKET_ADDR socket_addr;
    struct timeval select_timeout;
    fd_set rset;
    char logMsg[BUFFER_SIZE];

    void init();

    void log();

public:
    SocketHelper();

    ~SocketHelper();

    Status createSocket(char *ip, int port);

    Status connectSocket();

    Status sendMessage(char *buffer);

    Status sendHttpPostMsg(char *buffer);

    Status sendHttpGetMsg(char *buffer);

    Status recvMessage(char *buffer);

    Status closeSocket();
};

#endif //NDKSAMPLE_SOCKET_H
