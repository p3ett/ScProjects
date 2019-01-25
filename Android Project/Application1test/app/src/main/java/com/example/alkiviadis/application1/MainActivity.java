package com.example.alkiviadis.application1;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Button;

import com.example.alkiviadis.application1.mainactivitylisteners.ManualAutomaticListener;
import com.example.alkiviadis.application1.mainactivitylisteners.OnlineOfflineListener;
import com.example.alkiviadis.application1.mainactivitylisteners.StartStopListener;
import com.example.alkiviadis.application1.broker.PublishSubscribe;
import com.example.alkiviadis.application1.dialogs.ExitDialog;
import com.example.alkiviadis.application1.sensorcontrol.SensorNameSpace;

import mobileid.IdFinder;
import state.CurrentState;
import state.MobileState;
import android.provider.Settings.Secure;


public class MainActivity extends AppCompatActivity {   //first activity to be created when app is started
    final String TAG = this.getClass().getName(); // For BackPressedButton

    private CurrentState currentState;
    private MobileState mobileState;

    public CurrentState getCurrentState() {
        return currentState;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState); //creates MainActivity
        setContentView(R.layout.activity_main); //defines the xml file for the layout of MainActivity

        Drawable drawable = ContextCompat.getDrawable(getApplicationContext(), R.drawable.blind_icon);   //AppImage

        ActionBar supportActionBar = getSupportActionBar(); //creates the action bar on top of the screen

        if (supportActionBar != null) {
            getSupportActionBar().setDisplayShowHomeEnabled(true);
            getSupportActionBar().setIcon(drawable);
            getSupportActionBar().setLogo(drawable);
            getSupportActionBar().setDisplayUseLogoEnabled(true);
        }

        currentState = new CurrentState();
        mobileState = new MobileState();


        Button b = (Button) findViewById(R.id.buttonSwitch);    //creates the start/stop button
        b.setOnClickListener(new StartStopListener(b, this));    //sets a listener to react on this button

        Button b2 = (Button) findViewById(R.id.buttonManualAutomatic);    //creates the start/stop button
        b2.setOnClickListener(new ManualAutomaticListener(b2, this));    //sets a listener to react on this button

        Button b3 = (Button) findViewById(R.id.buttonOnlineOffline);    //creates the start/stop button
        b3.setOnClickListener(new OnlineOfflineListener(b3, this));    //sets a listener to react on this button

        // load settings from preferences to object: currentState


        // ----------------------------------------------
       // private String android_id = Secure.getString(getContext().getContentResolver(), Secure.ANDROID_ID);
       // IdFinder finder = new IdFinder();
        String id = Secure.getString(getContext().getContentResolver(), Secure.ANDROID_ID);

        mobileState.setId(id);

        Log.i("DEBUG", id);
    }

    public boolean onCreateOptionsMenu(Menu menu) {     //Creates the Options Menu
        MenuInflater inflater = getMenuInflater();  //Constructs a menu inflater
        inflater.inflate(R.menu.main_menu, menu);   //Inflate a menu hierarchy from the specified XML resource.
        return true;
    }

    //goes to the activity the user selected on the Options Menu(Settings,About,Exit)
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.idMnuAbout:       //user selected About
                //creates new Intent and stores the current data to its Bundle object
                //to be sent to desired activity
                Intent aboutIntent = new Intent(MainActivity.this, AboutActivity.class);

                startActivity(aboutIntent);
                break;
            case R.id.idMnuExit:    //user chooses to finish the application
                finish();
                SensorNameSpace.controller.stop();  //monitoring of sensors stops
                break;
            case R.id.idMnuSettings:    //user chooses Settings
                Intent settingsIntent = new Intent(MainActivity.this, SettingsActivity.class);
                //add to currentState the changes the user made to the Settings
                //Extras are retrieved on the Settings activity
                settingsIntent.putExtra("x", currentState);
                settingsIntent.putExtra("y", mobileState);

                //starts activity and expects a result from SettingsActivity
                //requestCode set to 0-->0 will be returned in onActivityResult() when activity exits
                startActivityForResult(settingsIntent, 0);
                break;
            case R.id.bla:
                PublishSubscribe s = new PublishSubscribe();
                s.publish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    //when Android Back Button is pressed
    @Override
    public void onBackPressed() {
        Log.d(TAG, "back pressed!");

        ExitDialog exitDialog = new ExitDialog();
        AlertDialog alertDialog = exitDialog.build(this);

        alertDialog.show();
    }

    //receives the result from called activity of startActivityForResult()
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        try {
            if (resultCode == RESULT_OK) {
                //saved the received changes to currentState in byte stream form
                currentState = (CurrentState) data.getSerializableExtra("x");
                mobileState = (MobileState) data.getSerializableExtra("y");
            }
        } catch (Exception e) { //normal flow of the program is disrupted

        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // save object currentState to shared preferences
    }
}
