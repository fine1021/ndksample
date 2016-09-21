package com.example.fine.ndksample.ndkInterface;

/**
 * HttpUtil Native Method
 */
public class HttpUtil {

    private static final int FLAG_BASE = 1;

    public static final int PORT = 80;

    public static final int FLAG_CONVERT = FLAG_BASE;                  // convert domain name to ip

    public static final int FLAG_NOT_CONVERT = FLAG_BASE << 1;         // not convert

    private static native String doPostRequest(String host, int port, String url, String content, int flag);

    private static native String doGetRequest(String host, int port, String url, int flag);

    private static native String doSocketConnect(String host, int port, String content, int flag);

    private static native void setCallback(Object object);

    static {
        System.loadLibrary("notify");
        System.loadLibrary("socket");
    }

    public static void socketCallback(Object object) {
        setCallback(object);
    }

    public static String socketConnect(String host, String content) {
        return doSocketConnect(host, PORT, content, FLAG_CONVERT);
    }

    public static String socketGetRequest(String host, String url) {
        return doGetRequest(host, PORT, url, FLAG_CONVERT);
    }

    public static String socketGetRequest(String host, int port, String url) {
        return doGetRequest(host, port, url, FLAG_CONVERT);
    }

    public static String socketPostRequest(String host, String url, String content) {
        return doPostRequest(host, PORT, url, content, FLAG_CONVERT);
    }

}
