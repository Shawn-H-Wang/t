 #include <stdio.h>
 #include <stdlib.h>
  
   int main(int argc, char *argv[])
  {
      if(argc == 2)//将文件中的打印到标准输出
      {
          FILE *fp;
          if( !(fp = fopen( argv[1], "r" )) )
         {
             fprintf(stderr,"cannot open file %s.",argv[1]);
             exit(-1);
         }
         int c;
         while( (c = fgetc(fp) ) != EOF )
         {
             fputc(c,stdout);
         }
         fclose(fp);
     }
}
