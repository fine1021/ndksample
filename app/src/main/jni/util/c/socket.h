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

#define OK 1
#define ERROR 0
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
char logMsg[BUFFER_SIZE];

#ifdef __cplusplus
extern "C" {
#endif

void initValue() {
    socketfd = -1;
    bzero(&socket_addr, sizeof(socket_addr));
    memset(logMsg, 0, sizeof(logMsg));
}

void log() {
#if defined(DEBUG) && DEBUG == 1
    LOGI("%s", logMsg);
#endif
    memset(logMsg, 0, sizeof(logMsg));
}

Status initSocket(char *ip, int port) {

    initValue();
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: socketfd = %d", __func__, socketfd);
    log();

    socket_addr.addr4.sin_family = AF_INET;                 // AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);
    // socket_addr.addr4.sin_addr.s_addr = inet_addr(ip);   // in_addr_t
    // inet_aton(ip, &socket_addr.addr4.sin_addr);
    inet_pton(AF_INET, ip, &socket_addr.addr4.sin_addr);

    // LOGI("%s: ip = %s, port = %d", __func__, inet_ntoa(socket_addr.addr4.sin_addr), port);
    char buffer[64];
    inet_ntop(AF_INET, &socket_addr.addr4.sin_addr, buffer, sizeof(buffer));
    snprintf(logMsg, BUFFER_SIZE, "%s: ip = %s, port = %d", __func__, buffer, port);
    log();

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

    snprintf(logMsg, BUFFER_SIZE, "%s: connect OK !", __func__);
    log();

    return OK;
}

Status sendMessage(char *msg) {

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    char buffer[BUFFER_SIZE] = {0};
    snprintf(buffer, BUFFER_SIZE, "%s", msg);

    snprintf(logMsg, BUFFER_SIZE, "%s: msg = %s", __func__, msg);
    log();

    int ret = send(socketfd, buffer, strlen(buffer), 0);

    snprintf(logMsg, BUFFER_SIZE, "%s: count = %d", __func__, ret);
    log();

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

    snprintf(logMsg, BUFFER_SIZE, "%s: select ret = %d", __func__, ret);
    log();

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

    snprintf(logMsg, BUFFER_SIZE, "%s: count = %d, msg = %s", __func__, count, msg);
    log();

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

    snprintf(logMsg, BUFFER_SIZE, "%s: close socket !", __func__);
    log();

    return OK;
}


#ifdef __cplusplus
}
#endif
