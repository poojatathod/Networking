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
    if(fp=fopen("resolver","r"))
    {
     while(fgets(line,100,fp))
     {
      sscanf(line,"%s %s",name,tip);
      ip=strtok(tip,"\n");
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

    sock->conn_fd=socket(AF_INET,SOCK_DGRAM,0);
        if (sock->conn_fd == -1)
        {
            //fprintf(stdout,"Unable To Initiate Client Socket");
            //exit(1);
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
 int counter=1;

 while(1)
 {

     ipaddr=getNextIp(subip);
     subip++;
     if(subip==256)
         subip=1;

     //if(!alreadyIN(ipaddr))
     {

     sock=clientSocket(ipaddr,39994);
    
     sendto(sock->conn_fd , argv[1] , strlen(argv[1]) , 0 , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr) );
    
     if(counter%10==0)
     {
         sleep(1);
         counter=1;
     }
     else
         counter++;
    
     }

 }

 return 0;
}

