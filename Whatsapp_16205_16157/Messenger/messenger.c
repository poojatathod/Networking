#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

extern void listener(int,char **);
extern void sender(int,char **);

int messenger(int argc,char *argv[])
{

  system("clear");  
  int pid;

   pid=fork();
   
   if(pid==0)
   {
    listener(2,argv);       
   }
   else
   {
    sender(2,argv);   
   }

return 0;
}

