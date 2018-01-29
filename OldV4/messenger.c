#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>


int main(int argc,char *argv[])
{

  system("clear");  
  int pid;
  char usrname[30],*uname;
  char *myargv[]={NULL,NULL};
  FILE *fp;

   myargv[0]=argv[1];
   myargv[1]=argv[1];

   pid=fork();
   
   if(pid==0)
   {
    execve("sender",myargv,NULL);       
   }
   else
   {
    execve("listener",myargv,NULL);   
   }

return 0;
}




