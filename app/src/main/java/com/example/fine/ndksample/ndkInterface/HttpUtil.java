package com.example.fine.ndksample.ndkInterface;

/**
 * HttpUtil Native Method
 */
public class HttpUtil {

    private static native String doPostRequest(String host, int port, String url, String content);

    private static native String doGetRequest(String host, int port, String url);

    private static native String doSocketConnect(String host, int port, String content);

    static {
        System.loadLibrary("socket");
    }

    public static String socketConnect(String host, int port, String content) {
        return doSocketConnect(host, port, content);
    }

    public static String socketGetRequest(String host, int port, String url) {
        return doGetRequest(host, port, url);
    }

    public static String socketPostRequest(String host, int port, String url, String content) {
        return doPostRequest(host, port, url, content);
    }

}
