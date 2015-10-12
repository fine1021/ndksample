package com.example.fine.ndksample.ndkInterface;

/**
 * HttpUtil Native Method
 */
public class HttpUtil {

    private static native String doPostRequest(String host, int port, String url, String content, int transform);

    private static native String doGetRequest(String host, int port, String url, int transform);

    private static native String doSocketConnect(String host, int port, String content, int transform);

    static {
        System.loadLibrary("socket");
    }

    public static String socketConnect(String host, String content) {
        return doSocketConnect(host, 80, content, 1);
    }

    public static String socketGetRequest(String host, String url) {
        return doGetRequest(host, 80, url, 1);
    }

    public static String socketPostRequest(String host, String url, String content) {
        return doPostRequest(host, 80, url, content, 1);
    }

}
