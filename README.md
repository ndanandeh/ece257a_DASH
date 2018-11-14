This is a DASH video player using Google EXOPlayer library to eventually integrate with MobileInsight for efficient video streaming over LTE networks.

Used for ECE 257A Project UCSD Fall 2018

To use:
-Install Android Studio
-Install Necessary SDK Version(currently using API 23 Android 6.0)
-Create a text file on your device that has a maximum bit rate value stored inside of it
-Edit the string "filePath" in "playURL.java" to match the path of that file on your device.
-Run 

Known Issues:
-Audio continues to play in the background when application is closed... this can be dealt with later

Next Project Steps:
-Need to parse data from mobileinsight and map it into a maximum bit rate that we can pass to exoplayer
