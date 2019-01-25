package com.example.alkiviadis.application1.sensorcontrol;

/**
 * Created by alkiviadis on 21/12/16.
 */
public class MyCurrentLoctionListener implements LocationListener
public class GpsSpy {

    @Override
    public void onLocationChanged(Location location) {
        location.getLatitude();
        location.getLongitude();

        String myLocation = "Latitude = " + location.getLatitude() + " Longitude = " + location.getLongitude();

        //I make a log to see the results
        Log.e("MY CURRENT LOCATION", myLocation);

    }

}
