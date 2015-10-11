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
        switch (hptr->h_addrtype) {
            case AF_INET:
                pptr = hptr->h_addr_list;
                for (; *pptr != NULL; pptr++) {
                    char buffer[64];
                    inet_ntop(AF_INET, *pptr, buffer, sizeof(buffer));
                    LOGI("%s: address = %s", __func__, buffer);
                }
                break;
        }
    }

    /* address, for backward compatibility */
    inet_ntop(AF_INET, hptr->h_addr, address, sizeof(address));
    LOGI("%s: IP = %s", __func__, address);
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

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        LOGE("%s: socket create fail !", __func__);
        return ERROR;
    }

    LOGI("%s: socketfd = %d", __func__, socketfd);

    socket_addr.addr4.sin_family = AF_INET;                 // AF_INET6 for IPV6
    socket_addr.addr4.sin_port = htons(port);               // ntohs(network to host short)
    // socket_addr.addr4.sin_addr.s_addr = inet_addr(address);   // in_addr_t
    // inet_aton(address, &socket_addr.addr4.sin_addr);
    inet_pton(AF_INET, address, &socket_addr.addr4.sin_addr);

    // LOGI("%s: ip = %s, port = %d", __func__, inet_ntoa(socket_addr.addr4.sin_addr), port);
    char buffer[64];
    inet_ntop(AF_INET, &socket_addr.addr4.sin_addr, buffer, sizeof(buffer));
    LOGI("%s: ip = %s, port = %d", __func__, buffer, port);

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

    LOGI("%s: connect OK !", __func__);

    return OK;
}

Status SocketHelper::sendMessage(char *buffer) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    LOGI("%s: msg = %s", __func__, buffer);
    int ret = send(socketfd, buffer, strlen(buffer), 0);
    LOGI("%s: send  = %d", __func__, ret);

    return OK;
}

Status SocketHelper::sendHttpPostMsg(char *buffer) {
    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    /*
    char packet[PACKET_SIZE] = {0};
    char content[BUFFER_SIZE] = {0};
    int content_len = snprintf(content,BUFFER_SIZE,"username=1078041387@qq.com&password=1111111&lt=LT-474072-bpDvuFOMfT4dDqkk0Hu21fzdMbGfcG&execution=e2s1&_eventId=submit");
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "POST /account/login HTTP/1.1\r\n");*/
    //len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: text/html,application/xhtml+xml,*/*\r\n");
    /*
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip, deflate\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Language: zh-CN\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: passport.csdn.net\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Referer: http://passport.csdn.net/account/login\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: no-cache\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "DNT: 1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Length: %d\r\n\r\n", content_len);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "%s", content);

    LOGI("%s: \r\n%s", __func__, packet);*/


    /**
     * youdao http post sample
     */
    /*
    char packet[PACKET_SIZE] = {0};
    char content[BUFFER_SIZE] = {0};
    int content_len = snprintf(content,BUFFER_SIZE,"key=user_plugins&");
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "POST /profile/text/get?keyfrom=mdict.6.0.1.android&model=H30-T10&mid=4.4.2&imei=863654020071692&vendor=tencent&screen=720x1280&abtest=4&userid=nuaa_yxkang@163.com HTTP/1.1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: Dalvik/1.6.0 (Linux; U; Android 4.4.2; H30-T10 Build/HuaweiH30-T10)\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: dict.youdao.com\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: Keep-Alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Length: %d\r\n\r\n", content_len);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "%s", content);

    LOGI("%s: \r\n%s", __func__, packet);*/

    /**
     * china mobile post sample
     */
    char packet[PACKET_SIZE] = {0};
    char content[BUFFER_SIZE] = {0};
    int content_len = snprintf(content,BUFFER_SIZE,"versionid=2.1.2&session=MDAIEFJRQODLUGVC32UB5QC3&num=13567175635&channelid=1&channel=1");
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "POST /zjweb/MainBillInfo.do HTTP/1.1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: android-async-http/1.3.1 (http://loopj.com/android-async-http)\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: app.m.zj.chinamobile.com\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: Keep-Alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Length: %d\r\n\r\n", content_len);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "%s", content);

    LOGI("%s: \r\n%s", __func__, packet);

    int count = send(socketfd, packet, strlen(packet), 0);
    LOGI("%s: send  = %d", __func__, count);

    return OK;
}

Status SocketHelper::sendHttpGetMsg(char *buffer) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    /*
    char packet[PACKET_SIZE] = {0};
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "GET / HTTP/1.1\r\n");*/
    //len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept: */*\r\n");
    /*
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Encoding: gzip,deflate\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Accept-Language: zh-CN,zh\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: %s:%d\r\n", address, 80);
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: keep-alive\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Cache-Control: no-cache\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\n\r\n");

    LOGI("%s: \r\n%s", __func__, packet);*/

    /**
     * youdao http get sample
     */
    char packet[PACKET_SIZE] = {0};
    int len = 0;
    len += snprintf(packet, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "GET /appapi/userconfig?keyfrom=mdict.6.0.1.android&model=H30-T10&mid=4.4.2&imei=863654020071692&vendor=tencent&screen=720x1280&abtest=4&userid=nuaa_yxkang@163.com&username=nuaa_yxkang@163.com HTTP/1.1\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Host: dict.youdao.com\r\n");
    len += snprintf(packet + len, PACKET_SIZE > len ? PACKET_SIZE - len : 0, "Connection: Keep-Alive\r\n\r\n");

    LOGI("%s: \r\n%s", __func__, packet);
    int count = send(socketfd, packet, strlen(packet), 0);
    LOGI("%s: send  = %d", __func__, count);

    return OK;
}

Status SocketHelper::recvMessage(char *buffer) {

    snprintf(logMsg, BUFFER_SIZE, "%s", __func__);
    callJavaMethod();

    if (socketfd < 0) {
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    FILE *fp;
    fp = fopen("/sdcard/socket.txt", "w");

    select_timeout.tv_sec = 3;
    select_timeout.tv_usec = 0;
    int jump = 0;
    int ret = 0;
    int count = 0;
    int single = 0;
    char data[RECV_SIZE];
    bzero(buffer, sizeof(buffer));

    while (jump < 3) {
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
            }
        }
        usleep(500);
        jump++;
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

    int next = 1;
    int single = 0;
    int count = 0;
    char data[BUFFER_SIZE];
    bzero(data, sizeof(data));
    bzero(buffer, sizeof(buffer));
    while(next) {
        single = recv(socketfd, data, sizeof(data), 0);
        if(single > 0) {
            next = 1;
            strncat(buffer, data, single);
            count += single;
        } else {
            next = 0;
        }
        if(next == 1) {
            single = 0;
            bzero(data, sizeof(data));
            usleep(500);
        }
#ifdef ANDROID
        printf("%s: errno = %d msg = %s\n", __func__, errno, strerror(errno));
#endif
    }*/

    fputs(buffer, fp);
    fclose(fp);

    if (count > 0) {
        if (count > PACKET_SIZE) {
            LOGW("%s: receive message overflow !", __func__);
        }
        char *content;
        if ((content = strstr(buffer, "200 OK")) != NULL) {
            content = strstr(buffer, "\r\n\r\n");
            if (content != NULL) {
                content += 4;
                bzero(buffer, sizeof(buffer));
                //strcpy(buffer, content);    // this method will add '\0' automatic
                bcopy(content, buffer, strlen(content));
                buffer[strlen(content)] = '\0';
            }
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
        LOGE("%s: socket has't init !", __func__);
        return ERROR;
    }

    close(socketfd);

#ifdef WIN32
    WSACleanup();
#endif

    LOGI("%s: close socket !", __func__);

    return OK;
}