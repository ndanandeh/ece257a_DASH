//
// Created by nathan on 11/27/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

JNIEXPORT jobjectArray Java_com_ucsd_ece257_dashplayer_playURL_getTS(JNIEnv *env, jdouble time)
{
    FILE *fp;
    char sizeReading[2];
    char buffer[50];
    memset(buffer,'A',sizeof(buffer));
    int lastSize = 0;
    int size = 0;
    char fileName[] = "/storage/emulated/0/mobileinsight/log/test1.txt";
    char ch;

    //datafields that we need
    int nant =0;
    int MCS0 =0; // 1 - 16QAM. 2 - 64QAM. 3 - QPSK
    int MCS1 =0;
    int TBS0 =0;
    int TBS1 =0;
    int RB11 =0;
    int RB10 =0;
    int RB01 =0;
    int RB00 =0;
    double RSRQ =0;

    int isFirst = 1;
    int stop = 0;
    double firstTS;
    int currentPos = 0; //current position in dataArray
    jclass dataClass = (*env)->FindClass(env,"com/ucsd/ece257/dashplayer/data");
    jmethodID constructor = (*env)->GetMethodID(env, dataClass, "<init>", "(IIIIIIIIID)V");
    jobjectArray dataArr = (*env)->NewObjectArray(env,100, dataClass, (*env)->NewObject(env,dataClass,constructor, nant, MCS0, MCS1, TBS0, TBS1, RB11, RB10, RB01, RB00, RSRQ) );


    int i = -1;
    int k = 0;
    long int posCurr = 0;
    long int pos = SEEK_END;
    long int posLast = 0;
    long int tempPos = 0;
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("error!");
        return -1;
    }

    while(stop==0) {

        if(isFirst){
            fseek(fp,-1,SEEK_END);
        }
        else {
            fseek(fp, pos, 0);
        }


        //go to last complete line first
        ch = fgetc(fp);
        while (ch != '\n') {
            i--; // this is just used for debugging now.
            fseek(fp, -2, SEEK_CUR);
            ch = fgetc(fp);
        }

        pos = ftell(fp);

        //search for the message length 68 = PDSCH packet, 52 = Intra_Frequency packet
        //use the 'sg' in msg_len to search for it
        while (ch != EOF) {
            while (ch != 's') {
                i--; //debugging variable
                fseek(fp, -2, SEEK_CUR);
                ch = fgetc(fp);
            }
            tempPos = ftell(fp); //save the current file index to use later
            ch = fgetc(fp);
            if (ch != 'g') {
                fseek(fp, -2, SEEK_CUR);
                continue;
            }

            //move forwards to the length
            fseek(fp, tempPos + 7, 0);
            ch = fgetc(fp);
            sizeReading[0] = ch;
            ch = fgetc(fp);
            sizeReading[1] = ch;
            lastSize = size;
            size = atoi(sizeReading);
            tempPos = tempPos - 32;
            fseek(fp, tempPos, 0); //point to the end of the previous line
            ch = fgetc(fp);
            ch = fgetc(fp);
            ch = fgetc(fp);
            ch = fgetc(fp);
            ch = fgetc(fp);
            ch = fgetc(fp);
            fseek(fp, tempPos, 0); //point to the end of the previous line


            //check if reading 2 different messages. NOTE: PDSCH must always be size 68.
            if (size != lastSize && lastSize != 0 && (size == 68 || lastSize == 68)) {
                break;
            } else {
                posLast = pos - 2;
                pos = tempPos - 2;
            }

        }

        posCurr = pos;
        //now pos and posLast point to the end of the two packet lines (1 PDSCH 1 inter-frequency)

        //NOW EXTRACT DATA

        for (int superLoop = 0; superLoop < 2; superLoop++) {
            //if the first one is PDSCH
            if (size == 68) {

                //get Timestamp FIRST and check if it is inside of desired range.
                fseek(fp, posCurr, 0);
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'm') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'p') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }
                    for (int jj = 0; jj < 19; jj++)
                        ch = fgetc(fp);

                    ch = fgetc(fp);
                    int ii = 0;
                    while (ch != '<') {
                        buffer[ii] = ch;
                        ii++;
                        ch = fgetc(fp);
                    }

                    //create temp buffer do char to double conversion
                    char *tempBuff = (char *) malloc(ii + 1);
                    for (int jj = 0; jj < ii; jj++) {
                        tempBuff[jj] = buffer[jj];
                    }
                    tempBuff[ii] = '\n'; //put non-number here to guarantee end of number

                    double ts = strtod(tempBuff, NULL);
                    if(isFirst){
                        firstTS = ts;
                        isFirst = 0;
                    }
                    else{
                        if((firstTS-ts) > time){
                            stop = 1;
                        }
                    }
                    free(tempBuff);
                    tempBuff = NULL;

                    break;
                }


                //get MCS1
                fseek(fp, posCurr, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'C') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'S') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    if (ch == 'Q') {
                        MCS1 = 3; //QPSK
                    } else if (ch == '1') {
                        MCS1 = 1; //16QAM
                    } else if (ch == '6') {
                        MCS1 = 2; //64QAM
                    }
                    break;
                }





                //get TBS1
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'B') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'S') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    int ii = 0;
                    while (ch != '<') {
                        buffer[ii] = ch;
                        ii++;
                        ch = fgetc(fp);
                    }

                    //create temp buffer do char to int conversion
                    char *tempBuff = (char *) malloc(ii + 1);
                    for (int jj = 0; jj < ii; jj++) {
                        tempBuff[jj] = buffer[jj];
                    }
                    tempBuff[ii] = '\n'; //put non-number here to guarantee end of number

                    TBS1 = atoi(tempBuff);
                    free(tempBuff);
                    tempBuff = NULL;
                    break;
                }




                //get MCS0
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'C') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'S') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    if (ch == 'Q') {
                        MCS0 = 3; //QPSK
                    } else if (ch == '1') {
                        MCS0 = 1; //16QAM
                    } else if (ch == '6') {
                        MCS0 = 2; //64QAM
                    }
                    break;
                }



                //get TBS0
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'B') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'S') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    int ii = 0;
                    while (ch != '<') {
                        buffer[ii] = ch;
                        ii++;
                        ch = fgetc(fp);
                    }

                    //create temp buffer do char to int conversion
                    char *tempBuff = (char *) malloc(ii + 1);
                    for (int jj = 0; jj < ii; jj++) {
                        tempBuff[jj] = buffer[jj];
                    }
                    tempBuff[ii] = '\n'; //put non-number here to guarantee end of number

                    TBS0 = atoi(tempBuff);
                    free(tempBuff);
                    tempBuff = NULL;
                    break;
                }


                //get RBAlloc 1[1]
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != '1') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != '[') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }
                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);

                    RB11 = 0;
                    while (ch != '<') {

                        if (ch == '0')
                            RB11 += 0;
                        else if (ch == '1' || ch == '2' || ch == '4' || ch == '8')
                            RB11 += 1;
                        else if (ch == '3' || ch == '5' || ch == '6' || ch == '9' || ch == 'a' ||
                                 ch == 'c')
                            RB11 += 2;
                        else if (ch == '7' || ch == 'b' || ch == 'd' || ch == 'e')
                            RB11 += 3;
                        else if (ch == 'f')
                            RB11 += 4;
                        else { //invalid character parsed
                            RB11 = -1;
                            break;
                        }
                        ch = fgetc(fp);


                    }

                    break;
                }



                //get RBAlloc 1[1]
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != '1') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != '[') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    RB10 = 0;
                    while (ch != '<') {

                        if (ch == '0')
                            RB10 += 0;
                        else if (ch == '1' || ch == '2' || ch == '4' || ch == '8')
                            RB10 += 1;
                        else if (ch == '3' || ch == '5' || ch == '6' || ch == '9' || ch == 'a' ||
                                 ch == 'c')
                            RB10 += 2;
                        else if (ch == '7' || ch == 'b' || ch == 'd' || ch == 'e')
                            RB10 += 3;
                        else if (ch == 'f')
                            RB10 += 4;
                        else { //invalid character parsed
                            RB10 = -1;
                            break;
                        }

                        ch = fgetc(fp);


                    }

                    break;
                }


                //get RBAlloc 0[1]
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != '0') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != '[') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    RB01 = 0;
                    while (ch != '<') {

                        if (ch == '0')
                            RB01 += 0;
                        else if (ch == '1' || ch == '2' || ch == '4' || ch == '8')
                            RB01 += 1;
                        else if (ch == '3' || ch == '5' || ch == '6' || ch == '9' || ch == 'a' ||
                                 ch == 'c')
                            RB01 += 2;
                        else if (ch == '7' || ch == 'b' || ch == 'd' || ch == 'e')
                            RB01 += 3;
                        else if (ch == 'f')
                            RB01 += 4;
                        else { //invalid character parsed
                            RB01 = -1;
                            break;
                        }

                        ch = fgetc(fp);


                    }

                    break;
                }


                //get RBAlloc 0[0]
                fseek(fp, tempPos, 0); // point to end of 1st line
                while (1) {
                    ch = fgetc(fp);
                    while (ch != '0') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != '[') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    tempPos = ftell(fp) - 3;
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    ch = fgetc(fp);
                    RB00 = 0;
                    while (ch != '<') {

                        if (ch == '0')
                            RB00 += 0;
                        else if (ch == '1' || ch == '2' || ch == '4' || ch == '8')
                            RB00 += 1;
                        else if (ch == '3' || ch == '5' || ch == '6' || ch == '9' || ch == 'a' ||
                                 ch == 'c')
                            RB00 += 2;
                        else if (ch == '7' || ch == 'b' || ch == 'd' || ch == 'e')
                            RB00 += 3;
                        else if (ch == 'f')
                            RB00 += 4;
                        else { //invalid character parsed
                            RB00 = -1;
                            break;
                        }

                        ch = fgetc(fp);


                    }

                    break;
                }


                fseek(fp, tempPos, 0); // point to end of 1st line

                //get NANT
                while (1) {
                    ch = fgetc(fp);
                    while (ch != '(') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != 'M') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);

                    nant = ch - '0';
                    break;

                }

            }

                //is interfrequency packet
            else {


                //get RSRQ
                fseek(fp, posCurr, 0);
                while (1) {
                    ch = fgetc(fp);
                    while (ch != 'Q') {
                        i--;
                        fseek(fp, -2, SEEK_CUR);
                        ch = fgetc(fp);
                    }
                    ch = fgetc(fp);
                    if (ch != '(') {
                        fseek(fp, -3, SEEK_CUR);
                        continue;
                    }

                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);
                    ch = fgetc(fp);


                    int ii = 0;
                    while (ch != '<') {
                        buffer[ii++] = ch;
                        ch = fgetc(fp);
                    }


                    char *tempBuff = (char *) malloc(ii + 1);
                    for (int jj = 0; jj < ii; jj++) {
                        tempBuff[jj] = buffer[jj];
                    }
                    tempBuff[ii] = '\n'; //put non-number here to guarantee end of number

                    RSRQ = strtod(tempBuff, NULL);
                    RSRQ *= -1;
                    free(tempBuff);
                    tempBuff = NULL;
                    break;
                }

            }

            //switch to next one
            size = lastSize;
            posCurr = posLast;


        }




        //char * line = NULL;
        //size_t len = 0;

//    ssize_t read = getline(&line,&len,fp);


        //jobject myData = (*env)->NewObject(env,dataClass,constructor, nant, MCS0, MCS1, TBS0, TBS1, RB11, RB10, RB01, RB00, RSRQ);
        (*env)->SetObjectArrayElement(env, dataArr, currentPos,
                                      (*env)->NewObject(env, dataClass, constructor, nant, MCS0,
                                                        MCS1, TBS0, TBS1, RB11, RB10, RB01, RB00,
                                                        RSRQ));

        currentPos++;

        if(stop == 1){
            (*env)->SetObjectArrayElement(env, dataArr, 99,
                                          (*env)->NewObject(env, dataClass, constructor, currentPos, currentPos,
                                                            currentPos, currentPos, currentPos, currentPos, currentPos, currentPos, currentPos,
                                                            currentPos));
        }
        //set all fields of 99 to # of frames processed
    }


    return dataArr;
    //return 1.0;
}
