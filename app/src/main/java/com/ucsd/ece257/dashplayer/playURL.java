package com.ucsd.ece257.dashplayer;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.net.Uri;
import android.util.Log;
import android.widget.TextView;

import com.google.android.exoplayer2.ExoPlayerFactory;
import com.google.android.exoplayer2.SimpleExoPlayer;
import com.google.android.exoplayer2.offline.FilteringManifestParser;
import com.google.android.exoplayer2.source.dash.*;
import com.google.android.exoplayer2.source.dash.manifest.DashManifestParser;
import com.google.android.exoplayer2.source.smoothstreaming.manifest.StreamKey;
import com.google.android.exoplayer2.trackselection.AdaptiveTrackSelection;
import com.google.android.exoplayer2.trackselection.DefaultTrackSelector;
import com.google.android.exoplayer2.trackselection.TrackSelection;
import com.google.android.exoplayer2.trackselection.TrackSelector;
import com.google.android.exoplayer2.ui.PlayerView;
import com.google.android.exoplayer2.ui.SimpleExoPlayerView;
import com.google.android.exoplayer2.upstream.*;
import com.google.android.exoplayer2.util.Util;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Timer;

import static android.os.Build.*;

//This class plays the dash video linked by the URL
public class playURL extends AppCompatActivity {

    static {
        System.loadLibrary("obtain_RSRQ-lib");
        System.loadLibrary("obtain_Nant-lib");
        System.loadLibrary("obtain_RBalloc-lib");
        //System.loadLibrary("obtain_fullParse-lib");
//        System.loadLibrary("obtain_TS-lib");

    }

    private static final String filePath = "/storage/emulated/0/pdsch_and_intraFreq.txt"; //file to read data from. For now just store a number defining max bit rate here

    private DataSource.Factory dataSourceFactory; //Used to create data sources (our video)
    private PlayerView exoPlayerView; //view that the player is defined inside of
    private Intent intent;  //intent used to create this activity
    private BandwidthMeter bandwidthMeter; //used to estimate bandwidth? This may not be used...
    private DefaultTrackSelector.Parameters par; //parameters of the track selector (sets max bitrate)
    private DefaultTrackSelector trackSelector; //Track selector (selects which quality to use)
    private SimpleExoPlayer simpleExoPlayer;  //the player object itself
    private int maxBitRate = 300000; //default defined maximum bitrate
    private TextView bitRateText;


    //shared Library
    public native double getRSRQ();
    public native int getNant();
    public native int[] getRBalloc();
    //public native data[] getTS(double time);

    // These variables are for updating the program (every delay seconds)
    Handler h = new Handler();
    int delay = 2*1000; // ms
    Runnable runnable;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //get the intent used to create this activity
        intent = getIntent();
        Log.d("MESSAGEINTENT", String.valueOf( intent.hasExtra(InputURL.MY_URL)));
        setContentView(R.layout.activity_play_url);

        //link the exoplayerView variable to the view that is defined in the layout.xml file
        exoPlayerView = (PlayerView) findViewById(R.id.player_view);

    }

    @Override //method called after onCreate()
    public void onStart(){
        super.onStart();

        //int nant = getNant();
        // double rsrq = getRSRQ();
        //int [] rbs = getRBalloc();
      // Log.d("RBALLOC_DEBUG", Integer.toString(rbs[0]) + " " + Integer.toString(rbs[1]));

        // obtain RSRQ
//        if (rsrq == -1)
//        {
//            AlertDialog.Builder builder;
//            if (VERSION.SDK_INT >= VERSION_CODES.LOLLIPOP) {
//                builder = new AlertDialog.Builder(this, android.R.style.Theme_Material_Dialog_Alert);
//            } else {
//                builder = new AlertDialog.Builder(this);
//            }
//            builder.setTitle("Error")
//                    .setMessage("Obtain RSRQ Failed")
//                    .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
//                        public void onClick(DialogInterface dialog, int which) {
//
//                        }
//                    })
//                    .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
//                        public void onClick(DialogInterface dialog, int which) {
//
//                        }
//                    })
//                    .setIcon(android.R.drawable.ic_dialog_alert)
//                    .show();
//        }
//
//
//        // obtain Nant
//        if (nant == -1)
//        {
//            AlertDialog.Builder builder;
//            if (VERSION.SDK_INT >= VERSION_CODES.LOLLIPOP) {
//                builder = new AlertDialog.Builder(this, android.R.style.Theme_Material_Dialog_Alert);
//            } else {
//                builder = new AlertDialog.Builder(this);
//            }
//            builder.setTitle("Error")
//                    .setMessage("Obtain Nant Failed")
//                    .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
//                        public void onClick(DialogInterface dialog, int which) {
//
//                        }
//                    })
//                    .setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
//                        public void onClick(DialogInterface dialog, int which) {
//
//                        }
//                    })
//                    .setIcon(android.R.drawable.ic_dialog_alert)
//                    .show();
//        }
//
//        System.out.println("RSRQ is " + rsrq);
//
//        System.out.println("Nant is " + nant);
        //we should stop here if we failed to get RSRQ above


        //this gets all of the data from the last .1 seconds. test[99] also contains the # of frames taken
   //     data [] test = getTS(.1);

        initializePlayer(); //This is where all the nasty stuff happens




        //text field to show max bit rate on screen
        //bitRateText = (TextView)findViewById(R.id.textView);

//        //Thread to Select Video Quality every delay seconds
//        h.postDelayed( runnable = new Runnable() {
//            //Function currently selects bit rate that is present in file at filePath
//            public void run() {
//                //count ++;
//                //Log.d("DEBUGTHREAD","Called " + Integer.toString(count));
//
//               int [] rbs = getRBalloc();
//               Log.d("RB_THREAD", Integer.toString(rbs[0]) + " " + Integer.toString(rbs[1]));
//
//                h.postDelayed(runnable,delay);
//
//            }
//        }, delay);

    }

    public void onResume() {
        super.onResume();
    }

    public void onPause(){
        super.onPause();
    }

    public void onStop(){
        super.onStop();
    }


    //nasty library function calls to initialize player...
    //basically starts DASH player with URL inputted, and if no URL is inputted plays the aws link
    void initializePlayer(){


        dataSourceFactory  = new DefaultHttpDataSourceFactory("dashPlayer");

        exoPlayerView.requestFocus();

        //if the string is inputted then go to it, else play the default aws site
        String message;
        if(intent.hasExtra(InputURL.MY_URL)) {
            message = intent.getStringExtra(InputURL.MY_URL);
        }
        else{
            message = "http://rdmedia.bbc.co.uk/dash/ondemand/testcard/1/client_manifest-events.mpd";
        }
        Uri uri = Uri.parse(message);

        Log.d("URILOG",uri.getHost());
        Log.d("URILOG",uri.getPath());


        DashChunkSource.Factory dashChunkFactory = new DefaultDashChunkSource.Factory( new DefaultHttpDataSourceFactory("dashPlayer"));
        DashMediaSource.Factory dashMediaSourceFactory = new DashMediaSource.Factory(dashChunkFactory,dataSourceFactory);

        DashMediaSource dashMediaSource = dashMediaSourceFactory.createMediaSource(uri);

        bandwidthMeter = new myBandwidthMeter();

        Log.d("BANDWIDTH_EST", String.valueOf(bandwidthMeter.getBitrateEstimate()));

        TrackSelection.Factory trackSelectFactory = new AdaptiveTrackSelection.Factory(bandwidthMeter);
        trackSelector = new DefaultTrackSelector(trackSelectFactory);

        simpleExoPlayer = ExoPlayerFactory.newSimpleInstance(this, trackSelector);

        Log.d("WINDOW_DEBUG", Integer.toString((simpleExoPlayer.getCurrentWindowIndex())));

        int index = simpleExoPlayer.getCurrentWindowIndex();
        long dur = simpleExoPlayer.getDuration();
        Log.d("INDEX_DEBUG",String.valueOf(index));
        Log.d("INDEX_DEBUG",String.valueOf(dur));

        //par = new DefaultTrackSelector.ParametersBuilder().build();

        //trackSelector.setParameters(par);

        DefaultTrackSelector.Parameters p = trackSelector.getParameters();
        Log.d("BR_DEBUG", String.valueOf(p.maxVideoBitrate));

        exoPlayerView.setPlayer(simpleExoPlayer);

        simpleExoPlayer.prepare(dashMediaSource);

        simpleExoPlayer.setPlayWhenReady(true);

    }


    //Returns the contents of a file as a string
    private String getFileContent(String targetFilePath) {
        //File file = new File(targetFilePath);
        StringBuilder fileContentBuilder = new StringBuilder();

        try{
            FileInputStream fis = new FileInputStream (new File(targetFilePath));
            BufferedReader reader = new BufferedReader(new InputStreamReader(new DataInputStream(fis)));

            String line;
            while((line = reader.readLine())!= null){
                fileContentBuilder.append(line);
                //fileContentBuilder.append("\n");    //delimiter... is it needed?
            }
            fis.close();

        } catch (FileNotFoundException e) {
            Log.d("DEBUGTAG", "FileNotFoundException");
            e.printStackTrace();
        } catch (IOException e) {
            Log.d("DEBUGTAG", "IOEXCEPTION");
            e.printStackTrace();
        }

        return fileContentBuilder.toString();
    }





}




