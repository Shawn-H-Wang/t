#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <string.h>

#define MAXSIZE 100
void Createdir(char *);
int main(int argc,char *argv[])
{
//判断参数
        if(argc <= 1){

                printf("mkdir:Usage:dirstr\n");
                exit(1);
        }
//遍历参数，对每个参数进行操作
        while(--argc){
                argv++;
                Createdir(*argv);
        }
}

void Createdir(char *path)
{
        char data[MAXSIZE];
         //判断是否是当前目录或/目录
        if((strcmp(path,".") == 0) || (strcmp(path,"/")==0))
                return;
         //判断目录是否存在
        if(access(path,F_OK) == 0)
                return;
        else{
        //保存目录
                strcpy(data,path);
        //获取目录的父目录
                dirname(path);
        //递归执行
                Createdir(path);
        }
        //创建目录
        if(mkdir(data,777) == -1){
                perror("mkdir error");
                exit(1);
         }
        return;
}
