package com.example.fine.ndksample;

import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import com.example.fine.ndksample.ndkInterface.HttpUtil;
import com.example.fine.ndksample.ndkInterface.Messenger;
import com.yxkang.android.os.WeakReferenceHandler;
import com.yxkang.android.util.Logger;

public class MainActivity extends AppCompatActivity {

    private TextView current, console;
    private InternalHandler mHandler = new InternalHandler(this);
    private Messenger.MessageCallBack callBack = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        current = (TextView) findViewById(R.id.textView);
        console = (TextView) findViewById(R.id.console);
        findViewById(R.id.socketButton1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketConnect(1);
            }
        });
        findViewById(R.id.socketButton2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketConnect(2);
            }
        });
        findViewById(R.id.socketButton3).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketConnect(3);
            }
        });
        callBack = new Messenger.MessageCallBack() {
            @Override
            public void onCallBack(final String msg) {
                mHandler.sendMessage(mHandler.obtainMessage(1, msg));
            }
        };
        Messenger.addListener(callBack);
    }

    private void setCurrent(String text) {
        current.setText(text);
    }

    private void setConsole(String text) {
        console.setText(text);
    }

    private void socketConnect(final int type) {
        mHandler.sendMessage(mHandler.obtainMessage(2, ""));
        new Thread(new Runnable() {
            @Override
            public void run() {
                String msg = "android_native";
                String url;
                String content;
                switch (type) {
                    case 1:
                        /*content = "hello baidu";
                        msg = HttpUtil.socketConnect("www.baidu.com", content);*/
                        content = "hello sina";
                        msg = HttpUtil.socketConnect("sae.sina.com.cn", content);
                        break;
                    case 2:
                        /*url = "/appapi/userconfig?keyfrom=mdict.6.0.1.android&model=H30-T10&mid=4.4.2&imei=863654020071692&vendor=tencent&screen=720x1280&abtest=4&userid=nuaa_yxkang@163.com&username=nuaa_yxkang@163.com";
                        msg = HttpUtil.socketGetRequest("dict.youdao.com", url);*/
                        url = "/public/2.0/bmt/translate?client_id=YourApiKey&q=today%0Atomorrow&from=auto&to=auto";
                        msg = HttpUtil.socketGetRequest("openapi.baidu.com", url);
                        break;
                    case 3:
                        url = "/zjweb/MainBillInfo.do";
                        content = "versionid=2.1.2&session=MDAIEFJRQODLUGVC32UB5QC3&num=13567175635&channelid=1&channel=1";
                        msg = HttpUtil.socketPostRequest("app.m.zj.chinamobile.com", url, content);
                        /*url = "/langdetect";
                        content = "query=hello";
                        msg = HttpUtil.socketPostRequest("fanyi.baidu.com", url, content);*/
                        break;
                }
                Logger.Global.i("android_native", msg);
                mHandler.sendMessage(mHandler.obtainMessage(2, msg));
            }
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (callBack != null) {
            Messenger.removeListener(callBack);
        }
        mHandler.removeCallbacksAndMessages(null);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private static class InternalHandler extends WeakReferenceHandler<MainActivity> {

        public InternalHandler(MainActivity reference) {
            super(reference);
        }

        @Override
        protected void handleMessage(MainActivity mainActivity, Message message) {
            switch (message.what) {
                case 1:
                    mainActivity.setCurrent(message.obj.toString());
                    break;
                case 2:
                    mainActivity.setConsole(message.obj.toString());
                    break;
            }
        }
    }
}
