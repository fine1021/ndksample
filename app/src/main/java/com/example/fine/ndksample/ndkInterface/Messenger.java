package com.example.fine.ndksample.ndkInterface;

import java.util.ArrayList;

/**
 * Messenger
 */
@SuppressWarnings("ALL")
public class Messenger {

    /**
     * Lock used to modify the content of {@link #sCallBacks}. Any write operation
     * performed on the array should be synchronized on this lock.
     */
    private static final Object sMutex = new Object();

    private static ArrayList<MessageCallBack> sCallBacks = new ArrayList<>();

    public static void addListener(MessageCallBack callBack) {
        synchronized (sMutex) {
            if (!sCallBacks.contains(callBack)) {
                sCallBacks.add(callBack);
            }
        }
    }

    public static void removeListener(MessageCallBack callBack) {
        synchronized (sMutex) {
            if (sCallBacks.contains(callBack)) {
                sCallBacks.remove(callBack);
            }
        }
    }

    public void notifyMessage(String msg) {
        synchronized (sMutex) {
            for (MessageCallBack callBack : sCallBacks) {
                callBack.onCallBack(msg);
            }
        }
    }

    public interface MessageCallBack {
        void onCallBack(String msg);
    }
}
