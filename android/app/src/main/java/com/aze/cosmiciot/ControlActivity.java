package com.aze.cosmiciot;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.aze.cosmiciot.classes.Globals;

public class ControlActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);

        if(Globals.CurrentDevice == null){
            finish();
        }

        TextView textview = findViewById(R.id.textView);
        textview.setText(Globals.CurrentDevice.Name);
    }
}