#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

typedef struct socketData
{
    int listener_fd,conn_fd;
    struct sockaddr_in listener_addr, client_addr;
}socketData;





socketData * listenerSocket(int port) 
{
 socketData *sock;

 sock=(socketData *)malloc(sizeof(socketData));
 
 sock->listener_fd=socket(AF_INET,SOCK_STREAM,0);
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

 if(listen(sock->listener_fd,50) == -1)
 {
    fprintf(stdout,"Not Listening");
    exit(1);
 }

 return sock; 
}




int main(int argc, const char *argv[])
{
    socketData *sock;
    sock=listenerSocket(65302);
    char username[20],msg[1024];
    int r=5,addrlen,i=-1;
    
    addrlen=sizeof(sock->client_addr);

    
    while(1)
    {
        
        fprintf(stdout,"\033[%d;%df",1,60);                            
        fprintf(stdout,"                   ####Display####                 ");   //FIXED HEADERS FOR DISPLAY (FOR LOOK AND FEEL ONLY)
        fprintf(stdout,"\033[%d;%df",2,95);
        fprintf(stdout,"User Logged In:  %s",argv[0]);
        fprintf(stdout,"\033[%d;%df",4,90);
        fprintf(stdout,"###INBOX###    ");

        
        
        sock->conn_fd=accept(sock->listener_fd , (struct sockaddr *) &sock->client_addr  , &addrlen );
            if(sock->conn_fd == -1)
            {
                perror("Unable to Accept Connection");
            }



        memset(msg,'\0',1024);
        read(sock->conn_fd,msg,1024);
        fprintf(stdout,"\033[%d;%df",r,90);    // for displaying output on right portion of terminal so no overlapping with kbd
        fputs(msg,stdout);
        r++;

        close(sock->conn_fd);
   }

   return 0;
}


/*
int main()
{
    socketData *sock;
    sock=listenerSocket(8888);
    char msg[1024],*ipaddr;
    int addrlen;

    while(1)
    {
 
        memset(&sock->client_addr,'0',sizeof(sock->client_addr));
        addrlen=sizeof(sock->client_addr);
        sock->conn_fd=accept(sock->listener_fd , (struct sockaddr *) &sock->client_addr  , &addrlen );
            if(sock->conn_fd == -1)
            {
                fprintf(stdout,"Unable to Accept Connection");
            }


        
        memset(msg,'\0',1024);
        read(sock->conn_fd,msg,1024);
        //ipaddr=inet_ntoa(sock->client_addr.sin_addr);
        strcat(msg,"  Address: ");
        //strcat(msg,ipaddr);
        fputs(msg,stdout);

        memset(msg,'\0',1024);
        strcpy(msg,"Suraj");
        write(sock->conn_fd,msg,1024);

   }

   return 0;
}
*/
