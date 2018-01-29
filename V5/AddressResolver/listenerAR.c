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
    struct sockaddr_in listener_addr, client_addr;
}socketData;



int alreadyIN(char *useraddr)
{
    
    FILE *fp=NULL;
    char line[100];
    if(fp=fopen("resolver","r"))
    {
     while(fgets(line,100,fp))
     {
      if (strcmp(line,useraddr)==0)
      {
          fclose(fp);
          return 1;
      }
     }
     fclose(fp);
    }
    else
    {
        fp=fopen("resolver","w");
        fclose(fp);
        return 0;
    }

}





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

 /*
 if(listen(sock->listener_fd,50) == -1)
 {
    fprintf(stdout,"Not Listening");
    exit(1);
 }
*/

 return sock; 
}




int main(int argc, const char *argv[])
{
    socketData *sock;
    sock=listenerSocket(39994);
    char *ipaddr,temp[1024],username[20],msg[1024];
    int addrlen,i=-1;
    FILE *fp;
    
    addrlen=sizeof(sock->client_addr);

    
    while(1)
    {
        memset(msg,'\0',1024);
        recvfrom(sock->listener_fd,msg,1024,0, (struct sockaddr *) &sock->client_addr , (socklen_t *) &addrlen );
        
        ipaddr=inet_ntoa(sock->client_addr.sin_addr);
        strcat(msg," ");
        strcat(msg,ipaddr);
        strcat(msg,"\n");
        

        if(!alreadyIN(msg))
        {    
         fp=fopen("resolver","a+");
         fputs(msg,fp);
         fclose(fp);	
	    }
      


   }

   return 0;
}



