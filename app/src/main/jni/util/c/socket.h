#include "../log.h"

#ifdef ANDROID

#include <netinet/in.h>      // sockaddr_in struct
#include <sys/types.h>       // socket data type
#include <sys/socket.h>      // sockaddr struct
#include <sys/time.h>        // timeval
#include <unistd.h>          // close socket
#include <string.h>          // bzero
#include <arpa/inet.h>       // inet_addr htons inet_ntoa inet_aton
#include <netdb.h>           // hostent

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

int socketfd;
char address[128];         // IP address
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
    bzero(address, sizeof(address));
}

void log() {
    LOGI("%s", logMsg);
    memset(logMsg, 0, sizeof(logMsg));
}

Status getHostByName(char *host) {
    struct hostent *hptr;
    char **pptr;
    hptr = gethostbyname(host);
    if (hptr == NULL) {
        LOGE("%s: getHostByName fail !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: official name = %s", __func__, hptr->h_name);
    log();

    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
        snprintf(logMsg, BUFFER_SIZE, "%s: aliases = %s", __func__, *pptr);
        log();
        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++) {
                    char buffer[64];
                    inet_ntop(AF_INET, *pptr, buffer, sizeof(buffer));
                    snprintf(logMsg, BUFFER_SIZE, "%s: address = %s", __func__, buffer);
                    log();
                }
                break;
        }
    }

    /* address, for backward compatibility */
    inet_ntop(AF_INET, hptr->h_addr, address, sizeof(address));
    snprintf(logMsg, BUFFER_SIZE, "%s: IP = %s", __func__, address);
    log();
    return OK;
}

Status initSocket(char *host, int port) {

    initValue();

    if (getHostByName(host) == ERROR) {
        return ERROR;
    }

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: socketfd = %d", __func__, socketfd);
    log();

    socket_addr.addr4.sin_family = AF_INET;                 // AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);
    // socket_addr.addr4.sin_addr.s_addr = inet_addr(address);   // in_addr_t
    // inet_aton(address, &socket_addr.addr4.sin_addr);
    inet_pton(AF_INET, address, &socket_addr.addr4.sin_addr);

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

    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;
    FD_ZERO(&rset);
    FD_SET(socketfd, &rset);
    int jump = 0;
    int ret = -1;
    int count = -1;
    char data[BUFFER_SIZE];
    bzero(data, sizeof(data));

    while (jump < 3) {
        ret = select(socketfd + 1, &rset, NULL, NULL, &select_timeout);
        if (ret < 0) {
            LOGE("%s: socket receive failed !", __func__);
            break;
        } else if (ret == 0) {
            LOGW("%s: socket receive timeout !", __func__);
            break;
        } else {
            snprintf(logMsg, BUFFER_SIZE, "%s: select ret = %d", __func__, ret);
            log();
            if (FD_ISSET(socketfd, &rset)) {
                count += recv(socketfd, data, sizeof(data), 0);
                strncat(msg, data, strlen(data));
            }
        }
        sleep(1);
        jump++;
        ret = -1;
        bzero(data, sizeof(data));
    }

    if (count != -1) {
        snprintf(logMsg, BUFFER_SIZE, "%s: count = %d, msg = %s", __func__, count, msg);
        log();
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
