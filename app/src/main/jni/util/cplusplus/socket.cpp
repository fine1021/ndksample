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
    bzero(address, sizeof(address));
    isTransform = true;
    isDebug = true;
}

void SocketHelper::log() {
    if (isDebug) {
        LOGI("%s", logMsg);
    }
    bzero(logMsg, sizeof(logMsg));
}


void SocketHelper::callJavaMethod() {
    if (env == NULL || obj == NULL) {
        return;
    }
    notifyMessageObj(env, obj, logMsg);
    bzero(logMsg, sizeof(logMsg));
}

Status SocketHelper::getHostByName(char *host) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

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

Status SocketHelper::initEnv(JNIEnv *env1, jobject obj1) {
    env = env1;
    obj = obj1;
    return OK;
};

Status SocketHelper::setTransform(bool value) {
    isTransform = value;
    return OK;
}

Status SocketHelper::setDebug(bool value) {
    isDebug = value;
    return OK;
}

Status SocketHelper::createSocket(char *host, int port) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (isTransform) {
        if (getHostByName(host) == ERROR) {
            return ERROR;
        }
    } else {
        strncpy(address, host, strlen(host));
    }

    strcpy(address, "101.200.29.44");

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: socketfd = %d", __func__, socketfd);
    log();

    socket_addr.addr4.sin_family = AF_INET;                 // AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);               // ntohs(network to host short)
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

Status SocketHelper::connectSocket() {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

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

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    snprintf(logMsg, BUFFER_SIZE, "%s: msg = %s", __func__, buffer);
    log();

    int ret = send(socketfd, buffer, strlen(buffer), 0);
    snprintf(logMsg, BUFFER_SIZE, "%s: send  = %d", __func__, ret);
    log();

    return OK;
}

Status SocketHelper::sendHttpPostMsg(char *buffer) {
    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    char packet[PACKET_SIZE] = {0};
    char content[BUFFER_SIZE] = {0};
    int content_len = snprintf(content,BUFFER_SIZE,"username=1078041387@qq.com&password=12122&lt=LT-463067-Pi2kax7H27f23SDa7r5clKONpCC93n&execution=e2s1&_eventId=submit");
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "POST /account/login?ref=toolbar HTTP/1.1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip, deflate, sdch\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Language: zh-CN,zh;q=0.8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: passport.csdn.net\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Referer: https://passport.csdn.net/account/login?ref=toolbar\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: max-age=0\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Length: %d\r\n\r\n", content_len);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "%s", content);

    snprintf(logMsg, PACKET_SIZE, "%s: \r\n%s", __func__, packet);
    log();

    int ret = send(socketfd, packet, strlen(packet), 0);
    snprintf(logMsg, BUFFER_SIZE, "%s: send  = %d", __func__, ret);
    log();
    return OK;
}

Status SocketHelper::sendHttpGetMsg(char *buffer) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    char packet[PACKET_SIZE] = {0};
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "GET / HTTP/1.1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip, deflate, sdch\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Language: zh-CN,zh;q=0.8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: %s:%d\r\n", address, 80);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: max-age=0\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\n\r\n");

    snprintf(logMsg, PACKET_SIZE, "%s: \r\n%s", __func__, packet);
    log();

    int ret = send(socketfd, packet, strlen(packet), 0);
    snprintf(logMsg, BUFFER_SIZE, "%s: send  = %d", __func__, ret);
    log();

    return OK;
}

Status SocketHelper::recvMessage(char *buffer) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;
    FD_ZERO(&rset);
    FD_SET(socketfd, &rset);
    int jump = 0;
    int ret = 0;
    int count = -1;
    char data[BUFFER_SIZE];
    bzero(data, sizeof(data));
    bzero(buffer, sizeof(buffer));

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
                strncat(buffer, data, strlen(data));
            }
        }
        sleep(1);
        jump++;
        ret = 0;
        bzero(data, sizeof(data));
    }

    if (count != -1) {
        if (count > PACKET_SIZE) {
            LOGW("%s: receive message overflow !", __func__);
        }
        snprintf(logMsg, PACKET_SIZE, "%s: count = %d\r\n%s", __func__, count, buffer);
        log();
    } else {
        return ERROR;
    }

    return OK;
}

Status SocketHelper::closeSocket() {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    close(socketfd);

    snprintf(logMsg, BUFFER_SIZE, "%s: close socket !", __func__);
    log();

    return OK;
}