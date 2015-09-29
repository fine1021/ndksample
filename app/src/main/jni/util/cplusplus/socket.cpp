//
// Created by yexiaokang on 2015/9/29.
//

#include "socket.h"

SocketHelper::SocketHelper() {
    init();
}

SocketHelper::~SocketHelper() {
    init();
}

void SocketHelper::init() {
    socketfd = -1;
    bzero(&socket_addr, sizeof(socket_addr));
    bzero(logMsg, sizeof(logMsg));
}

void SocketHelper::log() {

#if defined(DEBUG) && DEBUG == 1
    LOGI("%s", logMsg);
#endif
    bzero(logMsg, sizeof(logMsg));
}

Status SocketHelper::createSocket(char *ip, int port) {

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

Status SocketHelper::connectSocket() {
    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    if (connect(socketfd, (struct sockaddr *) &socket_addr, sizeof(struct sockaddr)) == -1) {
        LOGE("%s: socket connect fail !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: connect OK !", __func__);
    log();

    return OK;
}

Status SocketHelper::sendMessage(char *buffer) {
    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: msg = %s", __func__, buffer);
    log();

    int ret = send(socketfd, buffer, strlen(buffer), 0);

    snprintf(logMsg, BUFFER_SIZE, "%s: count = %d", __func__, ret);
    log();

    return OK;
}

Status SocketHelper::sendHttpPostMsg(char *buffer) {
    return OK;
}

Status SocketHelper::sendHttpGetMsg(char *buffer) {
    return OK;
}

Status SocketHelper::recvMessage(char *buffer) {
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
            count = recv(socketfd, buffer, sizeof(buffer), 0);
        }
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: count = %d, msg = %s", __func__, count, buffer);
    log();

    if (count != -1) {
        buffer[count] = '\0';
    } else {
        return ERROR;
    }

    return OK;
}

Status SocketHelper::closeSocket() {
    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    close(socketfd);

    snprintf(logMsg, BUFFER_SIZE, "%s: close socket !", __func__);
    log();

    return OK;
}