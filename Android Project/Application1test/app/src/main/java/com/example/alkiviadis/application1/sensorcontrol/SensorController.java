package com.example.alkiviadis.application1.sensorcontrol;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.widget.TextView;

import com.example.alkiviadis.application1.R;

import state.CurrentState;

//starts and stops monitoring of sensors
public class SensorController {
    private Activity activity;
    private SensorManager sMgr;
    private Sensor proximitySensor;
    private Sensor accelerometerSensor;

    private ProximitySpy proxSpy;
    private AccelerometerSpy accSpy;
    private GpsSpy gpsSpy;

    private CurrentState state;


    public void init(Activity activity) {
        this.activity=activity;
        sMgr = (SensorManager)activity.getSystemService(activity.SENSOR_SERVICE);

        proximitySensor = sMgr.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        accelerometerSensor = sMgr.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }

    public void start(CurrentState state) {
        this.state = state;

        proxSpy = new ProximitySpy(state);

        TextView tv = (TextView)activity.findViewById(R.id.textviewStatistics);
        accSpy = new AccelerometerSpy(tv, activity, state);

        sMgr.registerListener(proxSpy, proximitySensor, state.getMilliseconds());
        sMgr.registerListener(accSpy, accelerometerSensor, state.getMilliseconds());

    }

    public void stop() {
        if (proxSpy != null) {
            sMgr.unregisterListener(proxSpy);
            proxSpy = null;
        }

        if (accSpy != null) {
            sMgr.unregisterListener(accSpy);
            accSpy = null;
        }

    }


}