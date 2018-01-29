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




int alreadyIN(char *useraddr)
{
    FILE *fp=NULL;
    char line[100],name[20],tip[20],*ip;
    if(fp=fopen("resolver.conf","r"))
    {
     while(fgets(line,100,fp))
     {
      sscanf(line,"%s %s",name,tip);
      ip=strtok(ip,"\n");
      if (strcmp(ip,useraddr)==0)
       {
         fclose(fp);
          return 1;
       }
     }
     fclose(fp);
    }
     return 0;

}




clientSocketData * clientSocket (char *ipaddr, int port)
{
    clientSocketData *sock;
    
    sock=(clientSocketData *) malloc (sizeof(clientSocketData));

    memset(&sock->listener_addr,'0',sizeof(sock->listener_addr));

    sock->conn_fd=socket(AF_INET,SOCK_STREAM,0);
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



char * getNextIp(int prev)
{
  char *ipaddr,subip[5];
  ipaddr=(char *)malloc(20);

  strcpy(ipaddr,"192.168.43.");
  prev++;
  if(prev==256)
      prev=0;

  sprintf(subip,"%d",prev);
  strcat(ipaddr,subip);
  return ipaddr;
}



int main(int argc, const char *argv[])
{
 char *ipaddr,username[20];
 clientSocketData *sock;
 char msg[1024];
 int subip=-1,i=-1;
 FILE *fp;
 
 fp=fopen("currentuser","r");
 fgets(username,20,fp);
 fclose(fp);

 while(1)
 {
    
     ipaddr=getNextIp(subip);
     subip++;

     if(!alreadyIN(ipaddr))
     {

     sock=clientSocket(ipaddr,5955);
    
     if ( connect( sock->conn_fd , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr)) != -1)
     {
     
     memset(msg,'\0',1024);
     read(sock->conn_fd,msg,1024);
     ipaddr=inet_ntoa(sock->listener_addr.sin_addr);

     strcat(msg," ");
     strcat(msg,ipaddr);
     strcat(msg,"\n");
     
     fp=fopen("resolver.conf","a+");       // username and address stored if not already 
     fputs(msg,fp);
     fclose(fp);
     
     memset(msg,'\0',1024);
     strcpy(msg,username);
     write(sock->conn_fd,msg,1024);
 

     }

     close(sock->conn_fd);
     }
    }

 return 0;
}



/*int main()
{
 
 clientSocketData *sock;
 char msg[1024],*ipaddr;
 int subip=0;
 
 while(1)
 { 
    if(subip<255)
    {
     ipaddr=getNextIp(subip);
     subip++;
    
    sock=clientSocket("192.168.43.5",8888);
     
    if ( connect( sock->conn_fd , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr)) < 0)
    {
        fprintf(stdout,"192.168.43.%d ",subip);
        fprintf(stdout,"Unable to connect or whatspp is not running there\n");
    }
    else
    {
    memset(msg,'\0',1024);
    strcpy(msg,"Pooja");
    write(sock->conn_fd,msg,1024);
    
    memset(msg,'\0',1024);
    read(sock->conn_fd,msg,1024);
    ipaddr=inet_ntoa(sock->listener_addr.sin_addr);
    strcat(msg,"   Address: ");
    strcat(msg,ipaddr);
    fputs(msg,stdout);
    fprintf(stdout,"\n");
    }
   }
 }

 return 0;
}
*/
