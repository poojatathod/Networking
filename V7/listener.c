#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

typedef struct socketData
{
    int listener_fd;
    struct sockaddr_in forward_addr,listener_addr, client_addr;
}socketData;


socketData * listenerSocket(int port) 
{
 socketData *sock;

 sock=(socketData *)malloc(sizeof(socketData));
 
 sock->listener_fd=socket(AF_INET,SOCK_DGRAM,0);
    if(sock->listener_fd==-1)
    {
        perror("Unable to initiate listener socket");
        exit(1);
    }

 memset(&sock->listener_addr,'0',sizeof(sock->listener_addr));     //fill addr with '0'

 sock->listener_addr.sin_family = AF_INET;
 sock->listener_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 sock->listener_addr.sin_port=htons(port);

 if(bind(sock->listener_fd , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr) ) == -1)
 {
     fprintf(stdout,"Address Binding Failed");
     exit(1);
 }

 return sock; 
}



char * getIpByNum(char *destNumber,char **directContacts,int *nd,char **indirectContacts,int *ni)
{
 int i,flag=0;
 char num[10],*ip;
 ip=(char *) malloc(50);

 for(i=0;i<*nd;i++)
 {
  memset(num,'\0',10);
  memset(ip,'\0',50);
  sscanf(directContacts[i],"%s %s",num,ip);
  if(strcmp(num,destNumber)==0)
      return ip;
 }
 
 for(i=0;i<*ni;i++)
 {
  memset(num,'\0',10);
  memset(ip,'\0',50);
  sscanf(indirectContacts[i],"%s %s",num,ip);
  if(strcmp(num,destNumber)==0)
      return ip;
 }

}


void forwardMsg(socketData *sock,char *destNumber,char *msg,char **directContacts,int *nd,char **indirectContacts,int *ni)
{
 
 memset(&sock->forward_addr,'0',sizeof(sock->listener_addr));
 sock->forward_addr.sin_port=htons(65302);
 sock->forward_addr.sin_family=AF_INET;
 sock->forward_addr.sin_addr.s_addr = inet_addr(getIpByNum(destNumber,directContacts,nd,indirectContacts,ni)) ;
 
 sendto(sock->listener_fd,msg,strlen(msg),0,(struct sockaddr *) &sock->forward_addr,sizeof(sock->forward_addr));
}


void addToContacts(char *msg,char *ip,char **directContacts,int *nd,char **indirectContacts,int *ni)
{
    int i,flag=0;
    char *temp,*line;

    temp=msg+2;
    line=(char *)malloc(100);
    sprintf(line,"%s %s",temp,ip);
    
    if(msg[0]=='D')
    {
      for(i=0;i<*nd;i++)
       if(strcmp(line,directContacts[i])==0)
           flag=1;
      if(flag==0)
      {
          directContacts[*nd]=line;
          *nd+=1;

      }
    }
    else if(msg[0]=='I')
    {
      for(i=0;i<*nd;i++)
       if(strcmp(line,directContacts[i])==0)
           flag=1;
      if(flag==0)
      {
          indirectContacts[*ni]=line;
          *ni+=1;
      }
    }
}



int listener(int argc, const char *argv[],char **directContacts,int *nd,char **indirectContacts,int *ni)
{
    socketData *sock;
    sock=listenerSocket(65302);
    char *tokMsg,username[20],msg[1024],destNumber[20];
    int pid,r=5,addrlen,i=-1;
    
    addrlen=sizeof(sock->client_addr);

    
    while(1)
    {
        
        fprintf(stdout,"\033[%d;%df",1,60);                            
        fprintf(stdout,"                   ####Display####                 ");   //FIXED HEADERS FOR DISPLAY (FOR LOOK AND FEEL ONLY)
        fprintf(stdout,"\033[%d;%df",2,95);
        fprintf(stdout,"User Logged In:  %s",argv[1]);
        fprintf(stdout,"\033[%d;%df",4,90);
        fprintf(stdout,"###INBOX###    ");

        
        memset(msg,'\0',1024);
        recvfrom(sock->listener_fd, msg , 1024 , 0 , (struct sockaddr *) &sock->client_addr , (socklen_t *) &addrlen );

        if(msg[0]=='D' || msg[0]=='I')
        {
            addToContacts(msg,inet_ntoa(sock->client_addr.sin_addr),directContacts,nd,indirectContacts,ni);
        }
        else
        {
         strcpy(destNumber,msg+(strlen(msg)-10));
         if(strcmp(destNumber,argv[1])==0)
         {
         tokMsg=strtok(msg,"~");
         fprintf(stdout,"\033[%d;%df",r,90);    // for displaying output on right portion of terminal so no overlapping with kbd
         fputs(tokMsg,stdout);
         r++;
         }
         else
         {
          forwardMsg(sock,destNumber,msg,directContacts,nd,indirectContacts,ni);
         }
        }

   }
return 0;
}

