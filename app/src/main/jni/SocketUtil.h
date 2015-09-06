#include "common.h"

#include <netinet/in.h>      // sockaddr_in
#include <sys/types.h>       // socket
#include <sys/socket.h>      // socket
#include <sys/time.h>        // timeval
#include <unistd.h>          // close socket
#include <string.h>          // bzero
#include <arpa/inet.h>       // close

#define OK 1
#define ERROR 0
#define BUFFER_SIZE 4096
typedef int Status;

typedef struct SOCKET_ADDR {
    union {
        struct sockaddr_in addr4; // IPV4
        struct sockaddr_in6 addr6; // IPV6
    };
} SOCKET_ADDR;

int socketfd;
SOCKET_ADDR socket_addr;
struct timeval select_timeout;
fd_set rset;

#ifdef __cplusplus
extern "C" {
#endif

void resetValue() {
    socketfd = -1;
    bzero(&socket_addr, sizeof(socket_addr));
}

Status initSocket(char *ip, int port) {

    resetValue();
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

#ifdef DEBUG
    LOGD("%s: socketfd = %d", __func__, socketfd);
#endif

    socket_addr.addr4.sin_family = AF_INET; // IPv4    AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);
    socket_addr.addr4.sin_addr.s_addr = inet_addr(ip);

#ifdef DEBUG
    LOGI("%s: ip = %s, port = %d", __func__, inet_ntoa(socket_addr.addr4.sin_addr), port);
#endif

    return OK;
}

Status connectSocket() {

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    if (connect(socketfd, (struct sockaddr *) &socket_addr,
                sizeof(struct sockaddr)) == -1) {
        LOGE("%s: socket connect fail !", __func__);
        return ERROR;
    }

#ifdef DEBUG
    LOGD("%s: connect OK !", __func__);
#endif
    return OK;
}

Status sendMessage(char *msg) {

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    char buffer[BUFFER_SIZE] = {0};
    snprintf(buffer, BUFFER_SIZE, "%s", msg);

#ifdef DEBUG
    LOGD("%s: msg = %s", __func__, msg);
#endif

    int ret = send(socketfd, buffer, strlen(buffer), 0);

#ifdef DEBUG
    LOGD("%s: count = %d", __func__, ret);
#endif

    return OK;
}

Status recvMessage(char *msg) {

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    int count = -1;

    select_timeout.tv_sec = 10;
    select_timeout.tv_usec = 0;
    FD_ZERO(&rset);
    FD_SET(socketfd, &rset);

    int ret = select(socketfd + 1, &rset, NULL, NULL, &select_timeout);

#ifdef DEBUG
    LOGI("%s: select ret = %d", __func__, ret);
#endif

    if (ret < 0) {
        LOGE("%s: socket receive fail !", __func__);
        return ERROR;
    } else if (ret == 0) {
        LOGW("%s: socket receive timeout !", __func__);
    } else {
        if (FD_ISSET(socketfd, &rset)) {
            count = recv(socketfd, msg, sizeof(msg), 0);
        }
    }

#ifdef DEBUG
    LOGI("%s: count = %d, msg = %s", __func__, count, msg);
#endif

    if (count != -1) {
        msg[count] = '\0';
    } else {
        return ERROR;
    }

    return OK;
}

Status closeSocket() {

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    close(socketfd);

#ifdef DEBUG
    LOGI("%s: close socket !", __func__);
#endif
    return OK;
}

#ifdef __cplusplus
}
#endif
