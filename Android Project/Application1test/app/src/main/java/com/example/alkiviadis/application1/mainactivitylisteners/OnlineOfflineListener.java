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

public class OnlineOfflineListener implements View.OnClickListener {
    private MainActivity mainActivity;
    private boolean online = false;
    private Button button;

    public OnlineOfflineListener(Button button, MainActivity mainActivity) {
        this.mainActivity=mainActivity;
        this.button=button;
    }

    @Override
    public void onClick(View v) {
        if (online == false) {
            online = true;

//            CurrentState state = mainActivity.getCurrentState();

            //start monitoring the sensors
//            SensorNameSpace.controller.init(mainActivity);
//            SensorNameSpace.controller.start(state);


            button.setText("you are working offline");
            button.setBackgroundColor(Color.RED);

            Log.i("SensorController", "Controller start()");

        } else if (online ==true) {
            online = false;

            button.setText("you are working online");
            button.setBackgroundColor(Color.GREEN);

//            SensorNameSpace.controller.stop();

            Log.i("SensorController", "Controller stop()");
        }
    }
}
