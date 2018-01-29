#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>


int main(int argc,char *argv[])
{

  int pid;
  char usrname[30],*uname;
  char *myargv[]={NULL,NULL};
  FILE *fp;

   system("clear");  


  
   myargv[0]=argv[1];
   myargv[1]=argv[1];

   pid=fork();
   
   if(pid==0)
   {
    execve("connectorAR",myargv,NULL);                 // Child code, Child process will be replaced by kbd
   }
   else
   {
    execve("listenerAR",myargv,NULL);           // Parent process will be replaced by display
   }

return 0;
}




