#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

extern void addressResolver(int,char **);
extern void messenger(int,char **);

int main(int argc,char *argv[])
{
  
   int pid;
   
   system("clear");  
 
   if(argc==2)
   {

   pid=fork();
   
   if(pid==-1)
   {
    fputs("Unable to initiate app\n",stdout);
    exit(1);
   }
   
   if(pid==0)
   {
    addressResolver(2,argv);       
   }
   else
   {
    messenger(2,argv);  
   }
 
   }
   else
       fputs("Specify Username: ./whatsapp <username>\n",stdout);

return 0;
}




