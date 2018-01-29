#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>


int main()
{

  int pid;
  char usrname[30],*uname;
  char *myargv[]={NULL,NULL};
  FILE *fp;

   system("clear");  


   fprintf(stdout,"Enter username: ");
   fgets(usrname,20,stdin);
   uname=strtok(usrname,"\n");
   
   fp=fopen("currentuser","w");
   fputs(uname,fp);
   fclose(fp);


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




