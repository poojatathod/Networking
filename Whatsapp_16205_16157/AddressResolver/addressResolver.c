#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

extern void connectorAR(int,char **);
extern void listenerAR(int,char **);

int addressResolver(int argc,char *argv[])
{

   int pid;

   pid=fork();
   
   if(pid==0)
   {
     connectorAR(2,argv);
   }
   else
   {
     listenerAR(2,argv);
   }

return 0;
}




