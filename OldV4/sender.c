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



char *getIp()
{
  char line[1024],ch[5],name[20],*tip,*ips[50];
  FILE *fp=NULL;
  int i=0;

  while(! (fp = fopen("AddressResolver/resolver","r")))
  {}

  if (fp)
  {
      
      
      fprintf(stdout,"\033[%d;%df",2,55);
      fprintf(stdout,"Select a contact (each time) to msg:  ");
      while(fgets(line,1024,fp))
      {
          tip=(char *)malloc(20);
          memset(tip,'\0',20);
          sscanf(line,"%s %s\n",name,tip);
          fprintf(stdout,"\033[%d;%df",(i+3),55);
          fprintf(stdout,"[%d] %s\n",(i+1),name);
          ips[i]=tip;
          i++;
          memset(line,'\0',1024);
      }
      memset(ch,'\0',5); 
      fgets(ch,5,stdin);
      i=atoi(ch);
      return ips[i-1];
  }
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




int main(int argc, const char *argv[])
{
 char *ipaddr,username[20];
 clientSocketData *sock;
 char msg[1024];
 FILE *fp;
 
 //fp=fopen("currentuser","r");
 //fgets(username,20,fp);
 //fclose(fp);

 while(1)
 {
    
     ipaddr=getIp();

     sock=clientSocket(ipaddr,65302);
    
     if ( connect( sock->conn_fd , (struct sockaddr *) &sock->listener_addr , sizeof(sock->listener_addr)) != -1)
     {
    
     memset(msg,'\0',1024);
     fgets(msg,1024,stdin);
     write(sock->conn_fd,msg,1024);
 

     }

     close(sock->conn_fd);
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
