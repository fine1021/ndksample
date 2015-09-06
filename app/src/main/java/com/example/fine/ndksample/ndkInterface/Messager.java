package com.example.fine.ndksample.ndkInterface;

import java.util.ArrayList;

/**
 * Messager
 */
public class Messager {

    private static final ArrayList<MessageCallBack> callBacks = new ArrayList<>();

    public static synchronized void addListener(MessageCallBack callBack) {
        if (!callBacks.contains(callBack)) {
            callBacks.add(callBack);
        }
    }

    public static synchronized void removeListener(MessageCallBack callBack) {
        if (callBacks.contains(callBack)) {
            callBacks.remove(callBack);
        }
    }

    public void notifyMessage(String msg) {
        synchronized (callBacks) {
            for (MessageCallBack callBack : callBacks) {
                callBack.onCallBack(msg);
            }
        }
    }

    public interface MessageCallBack {
        void onCallBack(String msg);
    }
}
