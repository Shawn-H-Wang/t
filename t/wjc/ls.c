#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>



int main(int argc,char **argv)
{
    char c;
    char *path = ".";
    int isL = 0;    //if have -l,set isL=1,or isL=0
    while((c=getopt(argc,argv,"l"))!=-1)
    {
        isL = 1;

    }
    //printf("%d:%d",argc,optind);
    if(argc > optind)
    {
        path = argv[optind];
    }
    //printf("%s",path);

    struct dirent* ent = NULL;
    DIR* dir = opendir(path);
    int newLine = 0;
    while((ent = readdir(dir)))
    {
        if(isL == 0)
        {
            if((ent->d_type == 4 || ent->d_type == 8) && ent->d_name[0]!='.')
            {
                printf("%-24s",ent->d_name);
                newLine++;
                if(newLine % 3 == 0) printf("\n");
            }
        }
        else
        {
            struct stat info;
            //get directory detail info             
            char temp[80];
            strcpy(temp,path);
            strcat(temp,"/");
            strcat(temp,ent->d_name);
            if(lstat(temp,&info) == -1)
            {
                printf("Cannot open the directory:%s",temp);
                break;
            }
            else
            {
                if(ent->d_name[0]!='.')
                {
                    char *uid_to_name(),*ctime(),*gid_to_name(),*filemode();
                    void mode_to_letters();
                    char modestr[11];

                    //transfer mode to letters
                    mode_to_letters(info.st_mode,modestr);

                    printf("%s",modestr);
                    printf("%4d",(int)info.st_nlink);
                    printf("%8s",uid_to_name(info.st_uid));
                    printf("%8s",gid_to_name(info.st_gid)); 
                    printf("%10ld  ",(long)info.st_size);           
                    printf("%.12s  ",4+ctime(&info.st_mtime));

                    printf("%s",ent->d_name);

                    static char linkPath[1024];

                    readlink(temp,linkPath,1024);
                    if(S_ISLNK(info.st_mode))
                    {
                        printf("->%s",linkPath);
                    }                   


                    printf("\n");
                }
            }
        }

    }
    closedir(dir);
    puts("");
}

void mode_to_letters(int mode,char str[])
{
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0] = 'd';     //directory
    if(S_ISCHR(mode)) str[0] = 'c';     //characteristic
    if(S_ISBLK(mode)) str[0] = 'b';     //block
    if(S_ISLNK(mode)) str[0] = 'l';     //link

    if(mode & S_IRUSR) str[1]= 'r';
    if(mode & S_IWUSR) str[2]= 'w';
    if(mode & S_IXUSR) str[3]= 'x';

    if(mode & S_IRGRP) str[4]= 'r';
    if(mode & S_IWGRP) str[5]= 'w';
    if(mode & S_IXGRP) str[6]= 'x';

    if(mode & S_IROTH) str[7]= 'r';
    if(mode & S_IWOTH) str[8]= 'w';
    if(mode & S_IXOTH) str[9]= 'x';
}

char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(),*pw_ptr;
    static char numstr[10];
    if((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(),*grp_ptr;
    static char numstr[10];
    if((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    return grp_ptr->gr_name;
}