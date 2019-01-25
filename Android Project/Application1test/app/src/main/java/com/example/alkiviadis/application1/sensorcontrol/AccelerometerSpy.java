package com.example.alkiviadis.application1.sensorcontrol;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import state.CurrentState;

public class AccelerometerSpy implements SensorEventListener {
    private TextView textView;
    private Activity activity;
    private CurrentState state;
    private Toast toast;

    //for audio message
    private final int duration = 3; // seconds
    private final int sampleRate = 8000;
    private final int numSamples = duration * sampleRate;
    private final double sample[] = new double[numSamples];
    private final double freqOfTone = 440; // hz

    private final byte generatedSnd[] = new byte[2 * numSamples];

    private AudioTrack audioTrack = null;

    public AccelerometerSpy(TextView textView, Activity activity, CurrentState state) {
        this.textView = textView;
        this.activity = activity;
        this.state = state;
        this.toast = Toast.makeText(activity, "WATCH OUT!", Toast.LENGTH_LONG);

        genTone();
    }

    void genTone(){
        // fill out the array
        for (int i = 0; i < numSamples; ++i) {
            sample[i] = Math.sin(2 * Math.PI * i / (sampleRate/freqOfTone));
        }

        // convert to 16 bit pcm sound array
        // assumes the sample buffer is normalised.
        int idx = 0;
        for (final double dVal : sample) {
            // scale to maximum amplitude
            final short val = (short) ((dVal * 32767));
            // in 16 bit wav PCM, first byte is the low order byte
            generatedSnd[idx++] = (byte) (val & 0x00ff);
            generatedSnd[idx++] = (byte) ((val & 0xff00) >>> 8);

        }

        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT, generatedSnd.length, AudioTrack.MODE_STATIC);
        audioTrack.write(generatedSnd, 0, generatedSnd.length);
        audioTrack.setLoopPoints(0, generatedSnd.length/4, -1);
    }



    @Override
    public void onSensorChanged(SensorEvent event) {
        float [] accevalues = event.values;
        float [] proxValues = ProximitySpy.values;

        String s1 = "Proximity sensor value: "  + proxValues[0];
        String s2 = "Accelerometer sensor value: \n"  + accevalues[0] + "\n" + accevalues[1] + "\n" + accevalues[2];

        String msg = s1 + "\n" + s2;

        textView.setText(msg);

        //Conflict Condition
        if (proxValues != null) {
            if (proxValues[0] <= state.getSensor1_thres() &&
                    Math.abs(accevalues[0]) >= state.getSensor2_thres_x() &&
                    Math.abs(accevalues[1]) >= state.getSensor2_thres_y() &&
                    Math.abs(accevalues[2]) >= state.getSensor2_thres_z()) {
                toast.show();

                //Loop audio until danger of conflict is gone
                if (!(audioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING)) {
                    audioTrack.setLoopPoints(0, generatedSnd.length/4, -1);
                    audioTrack.play();
                }
                Log.i("*******AT*******", "state = " + audioTrack.getState() + " " + audioTrack.getPlayState());
                //danger of conflict is gone
            } else {
                toast.cancel();

                if ((audioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING)) {
                    audioTrack.stop();
                }
            }
        } else {
            toast.cancel();

            if ((audioTrack.getPlayState() == AudioTrack.PLAYSTATE_PLAYING)) {
                audioTrack.stop();
            }
        }

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
