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




void showContacts()
{
  char line[1024],ch[5],name[20],*tip,*ips[50];
  FILE *fp=NULL;
  int i;

  while(1)
  {
   i=0;
  while(! (fp = fopen("resolver","r")))
  {}
 
  if (fp)
  {
      
      
      fprintf(stdout,"\033[%d;%df",2,55);
      fprintf(stdout,"Select a contact (each time) to msg:  ");
      fprintf(stdout,"\033[%d;%df",3,55);
      fprintf(stdout,"Type enter \"exit\" to terminate.");

      tip=(char *)malloc(20);
      
      while(fgets(line,1024,fp))
      {
          memset(tip,'\0',20);
          sscanf(line,"%s %s\n",name,tip);
          fprintf(stdout,"\033[%d;%df",(i+4),55);
          fprintf(stdout,"[%d] %s\n",(i+1),name);
          ips[i]=tip;
          i++;
          memset(line,'\0',1024);
      }
     fclose(fp);
     free(tip);
  }

  fprintf(stdout,"\033[%d;%df",5,0);
  sleep(2);

  }

}




char *getIp(int ch)
{
  char line[1024],name[20],*tip,*ips[50];
  FILE *fp=NULL;
  int i=0;

  while(! (fp = fopen("resolver","r")))
  {}

 
  if (fp)
  {
      
      
      
      while(fgets(line,1024,fp))
      {
          tip=(char *)malloc(20);
          memset(tip,'\0',20);
          sscanf(line,"%s %s\n",name,tip);
          ips[i]=tip;
          i++;
          memset(line,'\0',1024);
      }

      return ips[ch-1];
  }
}

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




int main(int argc, const char *argv[])
{
 char *ipaddr,username[20];
 clientSocketData *sock;
 char ch[5],msg[1024],msgWithUsername[1024];
 FILE *fp;
 int pid,i;

 pid=fork();
 if(pid==0)
 {
  showContacts();
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
        killpg(getpgid(getpid()),SIGQUIT);
        //exit(0);
     }
   
     i=atoi(ch);
      
     ipaddr=getIp(i);

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
     sendto(sock->conn_fd,msgWithUsername,strlen(msgWithUsername),0,(struct sockaddr *) &sock->listener_addr,sizeof(sock->listener_addr));


    }

 return 0;
}

