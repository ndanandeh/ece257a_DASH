package com.ucsd.ece257.dashplayer;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;


/* THIS IS THE MAIN ENTRY ACTIVITY,  where the user can input a URL to an mpd file to play */
public class InputURL extends AppCompatActivity {

    //Identification tag for the URL (passed along with the URL to the playURL activity
    public final static String MY_URL = "com.ucsd.ece257.mydashstream.MESSAGE";


    @Override // This is the method called on creation of activity
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_input_url);

        //Must request Permission from user to read files in internal file system
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 0);
            }
        }

    }

    //Callback method for button (defined in layout/activity_input_url.xml)
    // When button is pressed this method is called
    // This method sends starts the playURL activity and sends it the URL as a message
    public void sendURL(View view){
        Intent intent = new Intent(this, playURL.class);
        EditText editText = (EditText) findViewById(R.id.edit_message);
        String message = editText.getText().toString();

        //only pass the message to the play activity if it is not empty
        if(! message.equals("")) {
            intent.putExtra(MY_URL, message);
        }

        Log.d("DEBUG_TAG", message);
        Log.d("DEBUG_TAG",String.valueOf(message.equals("")));
        startActivity(intent);
    }







}
