package com.example.alkiviadis.application1.dialoglisteners;

import android.support.v7.app.AlertDialog;
import android.view.View;
import android.widget.Button;

import com.example.alkiviadis.application1.MainActivity;

//implements the Cancel button of ExitDialog and resumes the application
public class CancelListener implements View.OnClickListener {
    private MainActivity mainActivity;
    private boolean run = false;
    private Button button;
    private AlertDialog dialog;

    public CancelListener(Button button, MainActivity mainActivity, AlertDialog dialog) {
        this.mainActivity=mainActivity;
        this.button=button;
        this.dialog = dialog;
    }

    @Override
    public void onClick(View v) {
        dialog.cancel();;
    }
}
