#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define DATA "HEllo,welcome to the world of socket"

int main(int argc,char *argv[])
{
   int sockfd,portno;
   struct sockaddr_in serv_addr;
   struct hostent *hp;
   char buff[1024];

   sockfd =socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
   {
	perror("socket failed");
	exit(1);
   }

   serv_addr.sin_family = AF_INET;
  
   hp = gethostbyname(argv[1]);
   if(hp==0)
   {
	perror("gethostbyname failed");
 	exit(1);
   }

   memcpy(&serv_addr.sin_addr,hp->h_addr,hp->h_length);
   portno = atoi(argv[2]);
   serv_addr.sin_port = htons(portno);

   if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
   {
     perror("connect failed");
     exit(1);
   }
   
   if(send(sockfd,DATA ,sizeof(DATA),0)<0)
   {
	perror("send failed");
	close(sockfd);
	exit(1);
   }

    printf("Sent %s \n",DATA);
    close(sockfd);

  return 0;
}
