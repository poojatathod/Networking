#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

extern int listener(int,char **,char **,int *,char **,int *);
extern int sender(int,char **,char **,int *,char **,int *);

int main(int argc,char *argv[])
{
  char *directContacts[100],*indirectContacts[100];
  int *nd,*ni;
  nd=(int *)malloc(sizeof(int));
  ni=(int *)malloc(sizeof(int));
  *nd=*ni=0;



  system("clear");  
  int pid;

   pid=fork();
   
   if(pid==0)
   {
    listener(2,argv,directContacts,nd,indirectContacts,ni);       
   }
   else
   {
    sender(2,argv,directContacts,nd,indirectContacts,ni);   
   }

return 0;
}

