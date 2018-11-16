package com.ucsd.ece257.dashplayer;

import android.util.Log;

import com.google.android.exoplayer2.upstream.BandwidthMeter;

//currently looping through bandwidth's
public class myBandwidthMeter implements BandwidthMeter {

    private int currentBR;

    myBandwidthMeter(){
        currentBR = 10000;
    }

    // NOTE: THE LIMITING BANDWIDTH IS SET TO 75% OF THE BIT RATE ESTIMATE !!!!!!!!!!!!
    // THIS IS FOR QUALITY PURPOSES... IS THIS WHAT DASH ACTUALLY DOES?
    // in order to set to actual bitrate divide number by .75...
    @Override
    public long getBitrateEstimate() {
        Log.d("GOT_BITRATE","got bitrate");
        currentBR = (int)(283320 /.75) + 1;

        return currentBR;
    }
}
