package com.example.fine.ndksample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.example.fine.ndksample.ndkInterface.HttpUtil;

public class MainActivity extends AppCompatActivity {

    @SuppressWarnings("FieldCanBeLocal")
    private Button mSocketButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSocketButton = (Button) findViewById(R.id.socketButton);
        mSocketButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socketConnect();
            }
        });
    }

    /**
     * IP : www.baidu.com  --->  115.239.210.27
     */
    private void socketConnect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                HttpUtil.socketConnect("115.239.210.27", 80);
            }
        }).start();
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
}
