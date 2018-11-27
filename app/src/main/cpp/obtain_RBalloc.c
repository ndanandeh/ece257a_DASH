//
// Created by nathan on 11/20/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

JNIEXPORT int Java_com_ucsd_ece257_dashplayer_playURL_getRBalloc(void)
{
    //文件指针
    FILE *fp;
    int numf = 0;
    char reading[30];
    memset(reading,'A',sizeof(reading));
    //文件路径
    char fileName[] = "/storage/emulated/0/mobileinsight/log/test1.txt";
    //用来保存从文件读取的字符
    char ch;

    //文件内部指针 fseek()函数的偏移
    int i = -1;
    int k = 0;  //保存小数点位置
    //如果文件打开失败
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("error!");
        return -1;
    }

    //移动指针离文件结尾 1 字节处
    fseek(fp, i, SEEK_END);

    //读取一个字符
    ch = fgetc(fp);
    while(ch != EOF){
        while(ch!='1'){
            i--;
            fseek(fp, i, SEEK_END);
            ch = fgetc(fp);
        }
        ch = fgetc(fp);
        if(ch!= '['){
            continue;
        }
        ch = fgetc(fp);
        if(ch == '0'){
            break;
        }
    }

    ch = fgetc(fp);
    ch = fgetc(fp);
    ch = fgetc(fp);
    ch = fgetc(fp);
    ch = fgetc(fp);

    i = 0;
    while (ch!='<')
    {
        ch = fgetc(fp);
        reading[i++]=ch;
        if(ch == 'f'){
            numf++;
        }
    }


    //int result=(int)strtol(reading,NULL,16);
    //printf("%d\n",result);
    fclose(fp);
    return numf;
}
