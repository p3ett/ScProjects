package com.example.alkiviadis.application1.mainactivitylisteners;

import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.alkiviadis.application1.MainActivity;
import com.example.alkiviadis.application1.sensorcontrol.SensorNameSpace;

import state.CurrentState;

/**
 * Created by alkiviadis on 21/12/16.
 */

public class ManualAutomaticListener implements View.OnClickListener {
    private MainActivity mainActivity;
    private boolean manual = false;
    private Button button;

    public ManualAutomaticListener(Button button, MainActivity mainActivity) {
        this.mainActivity=mainActivity;
        this.button=button;
    }

    @Override
    public void onClick(View v) {
        if (manual == false) {
            manual = true;

//            CurrentState state = mainActivity.getCurrentState();
//
//            //start monitoring the sensors
//            SensorNameSpace.controller.init(mainActivity);
//            SensorNameSpace.controller.start(state);
//
            button.setText("automatic");
            button.setBackgroundColor(Color.BLUE);

            Log.i("SensorController", "Controller start()");

        } else if (manual ==true) {
            manual = false;

            button.setText("manual");
            button.setBackgroundColor(Color.GREEN);

//            SensorNameSpace.controller.stop();

            Log.i("SensorController", "Controller stop()");
        }
    }
}
