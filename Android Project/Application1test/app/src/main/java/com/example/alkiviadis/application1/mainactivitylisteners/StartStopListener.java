package com.example.alkiviadis.application1.mainactivitylisteners;

import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.alkiviadis.application1.MainActivity;
import com.example.alkiviadis.application1.sensorcontrol.SensorNameSpace;

import state.CurrentState;

//implements the START/STOP button of MainActivity
public class StartStopListener implements View.OnClickListener {
    private MainActivity mainActivity;
    private boolean run = false;
    private Button button;

    public StartStopListener(Button button, MainActivity mainActivity) {
        this.mainActivity=mainActivity;
        this.button=button;
    }

    @Override
    public void onClick(View v) {

        if (run == false) {
            run = true;

            CurrentState state = mainActivity.getCurrentState();

            //start monitoring the sensors
            SensorNameSpace.controller.init(mainActivity);
            SensorNameSpace.controller.start(state);

            button.setText("stop");
            button.setBackgroundColor(Color.RED);

            Log.i("SensorController", "Controller start()");

        } else if (run ==true) {
            run = false;

            button.setText("start");
            button.setBackgroundColor(Color.GREEN);

            SensorNameSpace.controller.stop();

            Log.i("SensorController", "Controller stop()");
        }
    }
}
