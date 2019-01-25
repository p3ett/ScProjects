package com.example.alkiviadis.application1;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.alkiviadis.application1.settingsactivitylisteners.ButtonApplyListener;

import state.CurrentState;
import state.MobileState;

public class SettingsActivity extends AppCompatActivity {
    private CurrentState currentState;
    private MobileState mobileState;

    public CurrentState getCurrentState() {
        return currentState;
    }

    public MobileState getMobileState() {
        return mobileState;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState); //creates SettingsActivity
        setContentView(R.layout.activity_settings); //defines the xml file for the layout of SettingsActivity

        //receives the data from MainActivity as sequence of bytes
        currentState = (CurrentState) getIntent().getSerializableExtra("x");
        mobileState = (MobileState) getIntent().getSerializableExtra("y");

        EditText temp;
        TextView tmp;

        String str1 = currentState.getMilliseconds() + "";
        String str2 = currentState.getSensor1_thres() + "";
        String str3 = currentState.getSensor2_thres_x() + "";
        String str4 = currentState.getSensor2_thres_y() + "";
        String str5 = currentState.getSensor2_thres_z() + "";
        String str6 = mobileState.getId();
        String str7 = currentState.getServer_ip();
        String str8 = currentState.getServer_port();

        temp = (EditText) findViewById(R.id.idT);
        temp.setText(str1);

        temp = (EditText) findViewById(R.id.idTH1);
        temp.setText(str2);

        temp = (EditText) findViewById(R.id.idTH2x);
        temp.setText(str3);

        temp = (EditText) findViewById(R.id.idTH2y);
        temp.setText(str4);

        temp = (EditText) findViewById(R.id.idTH2z);
        temp.setText(str5);

        tmp = (TextView) findViewById(R.id.textViewID);
        tmp.setText(str6);

        temp = (EditText) findViewById(R.id.idIP);
        temp.setText(str7);

        temp = (EditText) findViewById(R.id.idPORT);
        temp.setText(str8);

        Button tempButton;
        tempButton = (Button) findViewById(R.id.idButtonApply);

        tempButton.setOnClickListener(new ButtonApplyListener(this));
    }

    //saves the changed data to bundle
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        EditText temp;

        temp = (EditText) findViewById(R.id.idT);
        String a = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idTH1);
        String b = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idTH2x);
        String c = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idTH2y);
        String d = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idTH2z);
        String e = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idIP);
        String f = temp.getText().toString();

        temp = (EditText) findViewById(R.id.idPORT);
        String g = temp.getText().toString();

        outState.putString("a", a);
        outState.putString("b", b);
        outState.putString("c", c);
        outState.putString("d", d);
        outState.putString("e", e);
        outState.putString("f", f);
        outState.putString("g", g);
    }

    //used for passing the data from SettingsActivity back to MainActivity
    @Override
    protected void onRestoreInstanceState(Bundle inState) {
        super.onRestoreInstanceState(inState);

        String a = inState.getString("a");
        String b = inState.getString("b");
        String c = inState.getString("c");
        String d = inState.getString("d");
        String e = inState.getString("e");
        String f = inState.getString("f");
        String g = inState.getString("g");

        EditText temp;

        temp = (EditText) findViewById(R.id.idT);
        temp.setText(a);

        temp = (EditText) findViewById(R.id.idTH1);
        temp.setText(b);

        temp = (EditText) findViewById(R.id.idTH2x);
        temp.setText(c);

        temp = (EditText) findViewById(R.id.idTH2y);
        temp.setText(d);

        temp = (EditText) findViewById(R.id.idTH2z);
        temp.setText(e);

        temp = (EditText) findViewById(R.id.idIP);
        temp.setText(f);

        temp = (EditText) findViewById(R.id.idPORT);
        temp.setText(g);
    }
}
