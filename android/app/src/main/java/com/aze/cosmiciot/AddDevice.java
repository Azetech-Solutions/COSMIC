package com.aze.cosmiciot;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.aze.cosmiciot.classes.Globals;

public class AddDevice extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_device);

        EditText deviceNameTextBox = findViewById(R.id.editTextDeviceName);
        Button button = findViewById(R.id.addDeviceButton);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String DeviceName = deviceNameTextBox.getText().toString();

                if(!DeviceName.equals("")) {
                    Globals.AddDevice(getApplicationContext(), DeviceName);
                    finish();
                }

            }
        });

    }
}