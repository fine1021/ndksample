//
// Created by yexiaokang on 2015/9/29.
//

#include "socket.h"
#include "../notify/interface.h"

SocketHelper::SocketHelper() {
    init();
    loadJavaMethod();
}

SocketHelper::~SocketHelper() {
    init();
}

void SocketHelper::init() {
    socketfd = -1;
    bzero(&socket_addr, sizeof(socket_addr));
    bzero(logMsg, sizeof(logMsg));
    bzero(socketHost, sizeof(socketHost));
    bzero(socketIP, sizeof(socketIP));
    isConvert = true;
}

void SocketHelper::loadJavaMethod() {

    void *libm_handle = NULL;
    JavaMethodInterface *(*getInterface)(void);

    libm_handle = dlopen("libnotify.so", RTLD_NOW);
    if (libm_handle == NULL) {
        LOGE("%s: dlopen failed: %s", __func__, dlerror());
        return;
    }
    getInterface = (JavaMethodInterface *(*)(void)) dlsym(libm_handle, "getJavaMethodInterface");
    if (getInterface == NULL) {
        LOGE("%s: dlsym failed: %s", __func__, dlerror());
        dlclose(libm_handle);
        return;
    }
    javaMethodInterface = getInterface();
    javaMethodInterface->initJavaVM(javaVM);
    javaMethodInterface->setCallback(callbackObj);
}

void SocketHelper::callJavaMethod() {
    if (javaMethodInterface == NULL) {
        bzero(logMsg, sizeof(logMsg));
        return;
    }
    javaMethodInterface->notifyMessage(logMsg);
    bzero(logMsg, sizeof(logMsg));
}

int SocketHelper::formatString(char *str, int flag) {
    int offset = 0;
    if (flag == 1) {                       // application/json
        while (str[offset] != '{') {
            offset++;
        }
        int len = strlen(str);
        int i = len - 1;
        for (; i > 0; i--) {
            if (str[i] != '}') {
                str[i] = '\0';
            } else {
                break;
            }
        }
    }
    LOGI("%s: offset = %d", __func__, offset);
    return offset;
}

Status SocketHelper::getHostByName(char *host) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

#ifdef WIN32
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);  // load Winsocket DLL
    if (err != 0) {
        return ERROR;
    }
#endif

    struct hostent *hptr;
    char **pptr;
    hptr = gethostbyname(host);
    if (hptr == NULL) {
        LOGE("%s: getHostByName fail !", __func__);
        return ERROR;
    }

    LOGI("%s: official name = %s", __func__, hptr->h_name);

    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
        LOGI("%s: aliases = %s", __func__, *pptr);
    }
    switch (hptr->h_addrtype) {
        case AF_INET:
            for (pptr = hptr->h_addr_list; *pptr != NULL; pptr++) {
                char buffer[64];
                inet_ntop(AF_INET, *pptr, buffer, sizeof(buffer));
                LOGI("%s: IPv4 address = %s", __func__, buffer);
            }
            break;
    }

    /* address, for backward compatibility */
    inet_ntop(AF_INET, hptr->h_addr, socketIP, sizeof(socketIP));
    // LOGI("%s: IP = %s", __func__, socketIP);
    return OK;
}

Status SocketHelper::setConvert(bool value) {
    isConvert = value;
    return OK;
}

Status SocketHelper::createSocket(char *host, int port) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (strlen(host) >= sizeof(socketHost)) {
        LOGE("%s: socket host is too long !", __func__);
        return ERROR;
    }

    strncpy(socketHost, host, strlen(host));
    if (isConvert) {
        if (getHostByName(host) == ERROR) {
            return ERROR;
        }
    } else {
        strncpy(socketIP, host, strlen(host));
    }

    LOGI("%s: socketHost = %s", __func__, socketHost);

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

    LOGI("%s: socketfd = %d", __func__, socketfd);

    socket_addr.addr4.sin_family = AF_INET;                 // AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);               // ntohs(network to host short)
    // socket_addr.addr4.sin_addr.s_addr = inet_addr(socketIP);   // in_addr_t
    // inet_aton(socketIP, &socket_addr.addr4.sin_addr);
    inet_pton(AF_INET, socketIP, &socket_addr.addr4.sin_addr);

    // LOGI("%s: ip = %s, port = %d", __func__, inet_ntoa(socket_addr.addr4.sin_addr), port);
    char buffer[64];
    inet_ntop(AF_INET, &socket_addr.addr4.sin_addr, buffer, sizeof(buffer));
    LOGI("%s: socketIP = %s, socketPort = %d", __func__, buffer, port);

    return OK;
}

Status SocketHelper::connectSocket() {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    if (connect(socketfd, (struct sockaddr *) &socket_addr, sizeof(struct sockaddr)) == -1) {
        LOGE("%s: socket connect fail !", __func__);
        return ERROR;
    }

    LOGI("%s: socket connect ok !", __func__);

    return OK;
}

Status SocketHelper::sendMessage(char *content) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    // LOGI("%s: \r\n%s", __func__, content);

    int count = send(socketfd, content, strlen(content), 0);
    if (count == strlen(content)) {
        LOGI("%s: socket send  ok !", __func__);
    } else {
        LOGE("%s: socket send  fail !", __func__);
    }

    return OK;
}

Status SocketHelper::sendHttpPostMsg(char *url, char *content) {
    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    int len = 0;
    int content_len = strlen(content);
    char packet[PACKET_SIZE] = {0};

    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "POST %s HTTP/1.1\r\n", url);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded;charset=utf-8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: android-socket-http\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: %s\r\n", socketHost);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: */*\r\n");
    //len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip\r\n");   // tell server the client encoding format. if removed, server will return text directly
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: no-cache\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Length: %d\r\n\r\n", content_len);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "%s", content);

    // LOGI("%s: \r\n%s", __func__, packet);

    int count = send(socketfd, packet, len, 0);

    if (count == len) {
        LOGI("%s: socket send  ok !", __func__);
    } else {
        LOGE("%s: socket send  fail !", __func__);
    }

    return OK;
}

Status SocketHelper::sendHttpGetMsg(char *url) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    int len = 0;
    char packet[PACKET_SIZE] = {0};

    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "GET %s HTTP/1.1\r\n", url);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: %s\r\n", socketHost);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: */*\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Language: zh-CN,zh\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: no-cache\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n\r\n");

    // LOGI("%s: \r\n%s", __func__, packet);

    int count = send(socketfd, packet, len, 0);

    if (count == len) {
        LOGI("%s: socket send  ok !", __func__);
    } else {
        LOGE("%s: socket send  fail !", __func__);
    }

    return OK;
}

Status SocketHelper::recvMessage(char *buffer, int bufferSize) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    FILE *fp;
    fp = fopen("/sdcard/socket.txt", "w");

    select_timeout.tv_sec = 3;
    select_timeout.tv_usec = 0;
    int ret = 0;
    int count = 0;
    int single = 0;
    bool alive = true;
    char data[RECV_SIZE];
    bzero(buffer, bufferSize);

    while (alive) {
        FD_ZERO(&rset);
        FD_SET(socketfd, &rset);
        ret = 0;
        bzero(data, sizeof(data));
        ret = select(socketfd + 1, &rset, NULL, NULL, &select_timeout);
        if (ret < 0) {
            LOGE("%s: socket receive failed !", __func__);
            break;
        } else if (ret == 0) {
            LOGW("%s: socket receive timeout !", __func__);
            break;
        } else {
            LOGI("%s: select ret = %d", __func__, ret);
            if (FD_ISSET(socketfd, &rset)) {
                single = recv(socketfd, data, sizeof(data), 0);
                strncat(buffer, data, single);
                count += single;
                alive = single > 0 && count < bufferSize;
            }
        }
        usleep(500);
    }

    /**
     * set socket timeout
     */
    /*
#ifdef ANDROID
    select_timeout.tv_sec = 3;
    select_timeout.tv_usec = 0;
    // setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&select_timeout, sizeof(struct timeval));
    setsockopt(socketfd, SOL_SOCKET,SO_RCVTIMEO, (char *)&select_timeout, sizeof(struct timeval));
#endif

#ifdef WIN32
	int timeOut = 3000;      // 3 seconds
	// setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeOut, sizeof(timeOut));
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut, sizeof(timeOut));
#endif

    int count = 0;
    int single = 0;
    bool alive = true;
    char data[RECV_SIZE];
    bzero(data, sizeof(data));
    bzero(buffer, bufferSize);
    while(alive) {
        single = recv(socketfd, data, sizeof(data), 0);
        if(single > 0) {
            strncat(buffer, data, single);
            count += single;
            alive = count < bufferSize;
        } else {
            alive = false;
        }
        if(alive) {
            single = 0;
            bzero(data, sizeof(data));
            usleep(500);
        }
#ifdef ANDROID
        LOGI("%s: errno = %d msg = %s\n", __func__, errno, strerror(errno));
#endif
    }*/

    fputs(buffer, fp);
    fclose(fp);

    // LOGD("%s: \n%s", __func__, buffer);

    if (count > 0) {
        if (count >= bufferSize) {
            LOGW("%s: receive message overflow !", __func__);
            javaMethodInterface->notifyMessageWithObj("receive message overflow !");
        }
        char *p;
        if ((p = strstr(buffer, "HTTP/1.1")) != NULL) {
            char *code;
            code = (char *)malloc(3 * sizeof(char));
            strncpy(code, p + 9, 3);
            LOGD("Response Code : %d", atoi(code));
            free(code);
        } else {
            javaMethodInterface->notifyMessageWithObj("not found error !");
            LOGD("%s: \n%s", __func__, buffer);
            bzero(buffer, bufferSize);            // clear buffer, since the message is invalid
            return ERROR;
        }
        if (strstr(buffer, "HTTP/1.1 200 OK") != NULL) {
            int flag = 0;
            if ((p = strstr(buffer, "Content-Type: application/json")) != NULL) {
                flag = 1;
            }
            int length = -1;
            if ((p = strstr(buffer, "Content-Length: ")) != NULL) {
                char *code2;
                code2 = (char *)malloc(5 * sizeof(char));
                strncpy(code2, p + 16, 5);
                length = atoi(code2);
                LOGD("Content-Length : %d", length);
                free(code2);
            }
            char *content;
            if ((p = strstr(buffer, "\r\n\r\n")) != NULL) {
                p += 4;                        // skip "\r\n\r\n"
                int len = strlen(p) + 1;
                content = (char *) malloc(len * sizeof(char));
                if (content == NULL) {
                    LOGE("%s: content == NULL !", __func__);
                    return ERROR;
                }
                strcpy(content, p);                             // copy the content
                if(length > 0 && strlen(content) != length) {   // check receive length
                    LOGE("%s: receive length error !", __func__);
                    javaMethodInterface->notifyMessageWithObj("receive length error !");
                }
                int offset = formatString(content, flag);
                bzero(buffer, bufferSize);                      // clear buffer
                strcpy(buffer, content + offset);               // copy the content to buffer
                //bcopy(content + offset, buffer, strlen(content));
                buffer[strlen(content) - offset] = '\0';
                free(content);                                  // free content
            }
        } else {
            bzero(buffer, bufferSize);              // clear buffer, since the message is invalid
            return ERROR;
        }
    } else {
        return ERROR;
    }

    return OK;
}

Status SocketHelper::closeSocket() {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket is not initialized !", __func__);
        return ERROR;
    }

    close(socketfd);

#ifdef WIN32
    WSACleanup();
#endif

    LOGI("%s: close socket !", __func__);

    return OK;
}