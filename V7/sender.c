#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

typedef struct clientsocketData
{
    int conn_fd;
    struct sockaddr_in listener_addr;
}clientSocketData;

clientSocketData * clientSocket (char *ipaddr, int port)
{
    clientSocketData *sock;
    
    sock=(clientSocketData *) malloc (sizeof(clientSocketData));

    memset(&sock->listener_addr,'0',sizeof(sock->listener_addr));

    sock->conn_fd=socket(AF_INET,SOCK_DGRAM,0);
        if (sock->conn_fd == -1)
        {
            fprintf(stdout,"Unable To Initiate Client Socket");
            exit(1);
        }

    sock->listener_addr.sin_family = AF_INET;
    sock->listener_addr.sin_port = htons(port);
    sock->listener_addr.sin_addr.s_addr = inet_addr(ipaddr) ;

    return sock;
}



void sendNumberAddress(clientSocketData *sock,const char *selfNumber,char **directContacts,int *nd,char **indirectContacts,int *ni)
{
    FILE *fp=NULL;
    char *ipaddr,IP[50],packet[15];
    int i;

    while(1)
    {
    system("./.scanIPs > .IPs");
    fp=fopen(".IPs","r");
    while(fgets(IP,50,fp))
    {
        ipaddr=strtok(IP,"\n");
        sock=clientSocket(ipaddr,65302);

        memset(packet,'\0',15);
        strcpy(packet,"D:");
        strcat(packet,selfNumber);

        sendto(sock->conn_fd ,packet , strlen(packet) , 0 , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr) );
        
        
        for(i=0;i<*nd;i++)
        {
         memset(packet,'\0',15);
         strcpy(packet,"D:");
         strcat(packet,directContacts[i]);
         sendto(sock->conn_fd ,packet , strlen(packet) , 0 , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr) );
        }
        
        for(i=0;i<*ni;i++)
        {
         memset(packet,'\0',15);
         strcpy(packet,"I:");
         strcat(packet,indirectContacts[i]);
         sendto(sock->conn_fd ,packet , strlen(packet) , 0 , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr) );
        }
    }
    fclose(fp);
    sleep(5);  
   }
}


    



void showContacts(char **directContacts,int *nd,char **indirectContacts,int *ni)
{
  char name[10],ip[50];
  int i,j;

      
     while(1)
     {
      fprintf(stdout,"\033[%d;%df",2,55);
      fprintf(stdout,"Select a contact (each time) to msg:  ");
      fprintf(stdout,"\033[%d;%df",3,55);
      fprintf(stdout,"Type enter \"exit\" to terminate.");

      
      for(i=0;i<*nd;i++)
      {
          memset(name,'\0',10);
          memset(ip,'\0',50);
          sscanf(directContacts[i],"%s %s",name,ip);
          fprintf(stdout,"\033[%d;%df",(i+4),55);
          fprintf(stdout,"[%d] %s\n",(i+1),name);
      }

      i--;
      
      for(j=0;j<*ni;j++)
      {
          memset(name,'\0',10);
          memset(ip,'\0',50);
          sscanf(indirectContacts[j],"%s %s",name,ip);
          fprintf(stdout,"\033[%d;%df",(i+4),55);
          fprintf(stdout,"[%d] %s\n",(i+1),name);
      }
  
      fprintf(stdout,"\033[%d;%df",5,0);
      }
      sleep(2);

}






int sender(int argc, const char *argv[],char **directContacts,int *nd,char **indirectContacts,int *ni)
{
 char *ipaddr,*destNumber;
 clientSocketData *sock;
 char ch[5],msg[1024],msgWithUsername[1024];
 FILE *fp;
 int pid,i;

 pid=fork();
 if(pid==0)
 {
  showContacts(directContacts,nd,indirectContacts,ni);
 }
 
 pid=fork();
 if(pid==0)
 {
     sendNumberAddress(sock,argv[1],directContacts,nd,indirectContacts,ni);
 }

 while(1)
 {
     memset(ch,'\0',5); 
     fprintf(stdout,"\033[%d;%df",5,0);
     fgets(ch,5,stdin);
     
     fprintf(stdout,"\033[%d;%df",5,0);
     for(i=0;i<5;i++)
         fprintf(stdout," ");

     if(strcmp(ch,"exit")==0)
     {
         //system("sudo service network-manager start");
         killpg(getpgid(getpid()),SIGQUIT);
        //exit(0);
     }
   
     i=atoi(ch);
     
     if(i<*nd)
        sscanf(directContacts[i-1],"%s %s",destNumber,ipaddr);
     else
        sscanf(indirectContacts[(i-*nd)-1],"%s %s",destNumber,ipaddr);
     
     sock=clientSocket(ipaddr,65302);
    

    
     memset(msg,'\0',1024);
     strcpy(msgWithUsername,argv[1]);
     strcat(msgWithUsername," - ");


     fprintf(stdout,"\033[%d;%df",2,0);
     fgets(msg,1024,stdin);
     
     fprintf(stdout,"\033[%d;%df",2,0);
     for(i=0;i<strlen(msg);i++)
         fprintf(stdout," ");
     
     strcat(msgWithUsername,msg);
     strcat(msgWithUsername,"~");
     strcat(msgWithUsername,destNumber);

     sendto(sock->conn_fd,msgWithUsername,strlen(msgWithUsername),0,(struct sockaddr *) &sock->listener_addr,sizeof(sock->listener_addr));


    }

 return 0;
}

