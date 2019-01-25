package com.example.alkiviadis.application1.sensorcontrol;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;

import state.CurrentState;

public class ProximitySpy implements SensorEventListener {
    public static float [] values = new float[] { 0,0,0};
    private CurrentState state;

    public ProximitySpy(CurrentState state) {
        this.state = state;
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        values = event.values;
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
