package com.example.alkiviadis.application1.dialogs;

import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;

import com.example.alkiviadis.application1.dialoglisteners.CancelListener;
import com.example.alkiviadis.application1.dialoglisteners.OkayListener;
import com.example.alkiviadis.application1.MainActivity;
import com.example.alkiviadis.application1.R;

//the alert dialog that comes up when user presses Android Back Button or Exit prom Options Menu
public class ExitDialog {
    public AlertDialog build(MainActivity activity) {
        android.support.v7.app.AlertDialog.Builder dialogBuilder = new android.support.v7.app.AlertDialog.Builder(activity);
        LayoutInflater inflater = activity.getLayoutInflater();
        View dialogView = inflater.inflate(R.layout.exitlayout, null);
        dialogBuilder.setView(dialogView);
        dialogBuilder.setTitle("Exit application");

        AlertDialog alertDialog = dialogBuilder.create();

        Button b1  = (Button) dialogView.findViewById(R.id.ok);
        Button b2  = (Button) dialogView.findViewById(R.id.cancel);

        OkayListener okayListener = new OkayListener(b1, activity);
        b1.setOnClickListener(okayListener);

        CancelListener cancelListener  = new CancelListener(b2, activity, alertDialog);
        b2.setOnClickListener(cancelListener);

        return alertDialog;
    }
}
