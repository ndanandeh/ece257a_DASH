//
// Created by Max Xing on 11/19/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>


JNIEXPORT int Java_com_ucsd_ece257_dashplayer_playURL_getNant(void) {
    //文件指针
    FILE *fp;
    char reading[10];
    memset(reading,'A',sizeof(reading));
    //文件路径
    char fileName[] = "/sdcard/Download/test1.txt";



    //用来保存从文件读取的字符
    char ch;

    //文件内部指针 fseek()函数的偏移
    int i = -1;

    //如果文件打开失败
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("error!");
        return -1;
    }

    //移动指针离文件结尾 1 字节处
    fseek(fp, i, SEEK_END);

    int results[5];
    int j = 0;
    //读取一个字符
    while (1) {
        ch = fgetc(fp);
        while (ch != '(') {
            i--;
            fseek(fp, i, SEEK_END);
            ch = fgetc(fp);
        }
        ch = fgetc(fp);
        if (ch != 'M')
            continue;

        ch = fgetc(fp);
        ch = fgetc(fp);
        ch = fgetc(fp);
        ch = fgetc(fp);

        results[j] = ch - '0';
        j++;
        if (j == 5)
            break;
    }


//    i=0;
//
//    while (ch!='<')
//    {
//        ch = fgetc(fp);
//        reading[i++]=ch;
//
//    }


    //char *num=new char[i];
//    char num[i];
//
//    for(int j=0;j<i;j++){
//        num[j]=reading[j];
//    }
//    int result=atoi(num);
//    fclose(fp);
//    printf("%d\n",result);


    return 1;


}
