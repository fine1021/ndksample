package com.example.fine.ndksample.ndkInterface;

/**
 * HttpUtil Native Method
 */
public class HttpUtil {

    private static native String doPostRequest(String ip, int port);

    private static native String doGetRequest(String ip, int port);

    private static native String doSocketConnect(String ip, int port);

    static {
        System.loadLibrary("socket");
    }

    public static String socketConnect(String ip, int port) {
        return doSocketConnect(ip, port);
    }

}
