//
// Created by Max Xing on 11/15/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>

JNIEXPORT double Java_com_ucsd_ece257_dashplayer_playURL_getRSRQ(void)
{
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
    while(ch!='Q'){
      i--;
      fseek(fp, i, SEEK_END);
      ch = fgetc(fp);
   }
   ch=fgetc(fp);
   ch=fgetc(fp);
   ch=fgetc(fp);
   i=0;
   while (ch!='<')
    {
        ch = fgetc(fp);
        reading[i++]=ch;
        if(ch == '.') k=i;
    }
    i--;

    //char *num=new char[i];
    char num[i];

    for(int j=0;j<i;j++){
        num[j]=reading[j];
    }
    float result=atoi(num);
    result=result + 0.1*(reading[k]-'0');
   printf("%f\n",result);
    fclose(fp);
    return result;
}
