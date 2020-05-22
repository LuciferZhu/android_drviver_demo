package com.example.ledcontrol;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.lowlevel.LedNative;

public class MainActivity extends AppCompatActivity {
    private final String TAG = "LedControl";
    private Button btn_led = null;
    LedNative ledNative = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        ledNative = new LedNative();
        ledNative.openDev();
    }

    public void initView() {
        btn_led = (Button) findViewById(R.id.btn_led);
    }

    public void onClickLed(View v) {

        if (btn_led.getText().equals("开灯") ) {
            Log.d(TAG, "turn on led");
            btn_led.setText("关灯");
            ledNative.devOn();

        } else {
            Log.d(TAG, "turn off led");
            btn_led.setText("开灯");
            ledNative.devOff();

        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ledNative.closeDev();
    }
}
