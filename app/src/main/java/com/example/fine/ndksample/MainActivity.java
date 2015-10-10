package com.example.fine.ndksample;

import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.fine.ndksample.ndkInterface.HttpUtil;
import com.example.fine.ndksample.ndkInterface.Messenger;
import com.yxkang.android.os.WeakReferenceHandler;

public class MainActivity extends AppCompatActivity {

    private TextView textView;
    private InternalHandler mHandler = new InternalHandler(this);
    private Messenger.MessageCallBack callBack = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = (TextView) findViewById(R.id.textView);
        Button mSocketButton = (Button) findViewById(R.id.socketButton);
        mSocketButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketConnect();
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

    private void setTextView(final String text) {
        textView.setText(text);
    }

    private void socketConnect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                //HttpUtil.socketConnect("www.baidu.com", 80);
                //HttpUtil.socketConnect("dict.youdao.com", 80);
                HttpUtil.socketConnect("app.m.zj.chinamobile.com", 80);
                //HttpUtil.socketConnect("sae.sina.com.cn", 80);
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
            mainActivity.setTextView(message.obj.toString());
        }
    }
}
