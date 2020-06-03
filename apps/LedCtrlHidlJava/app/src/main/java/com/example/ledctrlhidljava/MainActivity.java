package com.example.ledctrlhidljava;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.os.RemoteException;
import my_demo.hardware.led.V1_0.ILed;

public class MainActivity extends Activity {
    private final String TAG = "LedCtrlHidlJava";
    private Button btn_led = null;
    private ILed led_server;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        try {
            Log.d(TAG, "get led hidl service:");
            led_server = ILed.getService(true);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        Log.d(TAG, "done");
    }

    public void initView() {
        btn_led = (Button) findViewById(R.id.btn_led);
    }

    public void onClickLed(View v) {
        int ret;

        try {
            if (btn_led.getText().equals("开灯") ) {
                Log.d(TAG, "turn on led");
                btn_led.setText("关灯");
                if ( led_server.control(1) < 0 ) {
                    Log.e(TAG, "led_server.control(0) failed");
                }

            } else {
                Log.d(TAG, "turn off led");
                btn_led.setText("开灯");
                if ( led_server.control(0) < 0 ) {
                    Log.e(TAG, "led_server.control(1) failed");
                }
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Exception in led_server.control() " + e);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
