package com.example.alkiviadis.application1.settingsactivitylisteners;

import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.example.alkiviadis.application1.R;
import com.example.alkiviadis.application1.SettingsActivity;

//save settings
public class ButtonApplyListener implements View.OnClickListener {
    private SettingsActivity settingsActivity;

    public ButtonApplyListener(SettingsActivity settingsActivity) {
        this.settingsActivity = settingsActivity;
    }

    @Override
    public void onClick(View v) {
        try {
            EditText temp;

            temp = (EditText) settingsActivity.findViewById(R.id.idT);
            String a = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idTH1);
            String b = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idTH2x);
            String c = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idTH2y);
            String d = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idTH2z);
            String e = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idIP);
            String f = temp.getText().toString();

            temp = (EditText) settingsActivity.findViewById(R.id.idPORT);
            String g = temp.getText().toString();

            int tempa = Integer.parseInt(a);
            float tempb = Float.parseFloat(b);
            float tempc = Float.parseFloat(c);
            float tempd = Float.parseFloat(d);
            float tempe = Float.parseFloat(e);
            String tempf = f;
            String tempg = g;

            settingsActivity.getCurrentState().setMilliseconds(tempa);
            settingsActivity.getCurrentState().setSensor1_thres(tempb);
            settingsActivity.getCurrentState().setSensor2_thres_x(tempc);
            settingsActivity.getCurrentState().setSensor2_thres_y(tempd);
            settingsActivity.getCurrentState().setSensor2_thres_z(tempe);
            settingsActivity.getCurrentState().setServer_ip(tempf);
            settingsActivity.getCurrentState().setServer_port(tempg);

            Toast.makeText(settingsActivity, "Settings saved", Toast.LENGTH_LONG).show();

            Intent returnIntent = settingsActivity.getIntent();
            returnIntent.putExtra("x",settingsActivity.getCurrentState());
            returnIntent.putExtra("y",settingsActivity.getMobileState());
            settingsActivity.setResult(settingsActivity.RESULT_OK,returnIntent);

            settingsActivity.finish();

        } catch (Exception e) {
            Toast.makeText(settingsActivity, "ERror " + e.getMessage(), Toast.LENGTH_LONG).show();
            Log.i("ERROR", e.toString());

            Toast.makeText(settingsActivity, "An error has occured while saving settings", Toast.LENGTH_LONG).show();
        }
    }
}
