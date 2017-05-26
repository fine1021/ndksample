package com.example.fine.ndksample;

/**
 * Created by yexiaokang on 2016/4/1.
 */
public class SystemProperties {

    public static String get(String key, String def) {
        return native_get(key, def);
    }

    private static native String native_get(String key, String def);

    static {
        System.loadLibrary("properties");
    }
}
