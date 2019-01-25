package com.example.alkiviadis.application1.dialoglisteners;

import android.view.View;
import android.widget.Button;

import com.example.alkiviadis.application1.MainActivity;
import com.example.alkiviadis.application1.sensorcontrol.SensorNameSpace;

//implements the OK button of ExitDialog and terminates the application
public class OkayListener implements View.OnClickListener {
    private MainActivity mainActivity;
    private boolean run = false;
    private Button button;

    public OkayListener(Button button, MainActivity mainActivity) {
        this.mainActivity=mainActivity;
        this.button=button;
    }

    @Override
    public void onClick(View v) {
        mainActivity.finish();
        SensorNameSpace.controller.stop();
    }
}
