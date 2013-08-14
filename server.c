#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h> 
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<netinet/if_ether.h>
#include<net/if.h>
#include<linux/sockios.h>
#include <sys/ioctl.h>
#include <fcntl.h>


int main(int argc,char *argv[])
{
 /*variables*/
   int sockfd,portno;
   struct sockaddr_in serv_addr,cli_addr;
   int mysock; // hold the connection when client connects.  
   char buff[1024];
   int rval,clilen;
 
  if(argc < 2)
  {	fprintf(stderr,"Error,no port provided \n"); }

  /*create socket*/

  sockfd = socket(AF_INET,SOCK_STREAM,0);
 
  if(sockfd <  0 ) {
	perror("failed to create socket");
   	exit(1);
   }
  
  bzero((char*) &serv_addr, sizeof(serv_addr)); //clearing complete structure. 
 
  portno = atoi(argv[1]); // converting portno from char to int.
 
  serv_addr.sin_family = AF_INET; //internet addr
  serv_addr.sin_addr.s_addr = INADDR_ANY; //u gt ur addr by own(PC addr)
  serv_addr.sin_port = htons(portno); // int to n/w format.
  

/*call bind*/

    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
    	perror("bind failed");
	exit(1);

    }
  
 /*listen*/
   listen(sockfd,5); // '5' is the no of clients server can handle.
 
 /*Accept*/
  
 do {
        int n;
	clilen = sizeof(cli_addr);
        mysock = accept(sockfd, (struct sockaddr *)&cli_addr , &clilen);
        if(mysock < 0)
        {
	  perror("accept failed");
        }
	else
 	{
   	 memset(buff, 0,sizeof(buff)); //resets memory buff to '0'.clearing buffer.//'rval' consists of no. of bytes recvd.
		
        if((rval = recv(mysock, buff , sizeof(buff), 0)<0))
	    perror("reading stream msg error");
  	
   	else{
		printf("MSG: %s\n",buff); }
	
	   printf("Got the message  \n");
	   printf("%s \n",inet_ntoa(cli_addr.sin_addr));
	   printf("%d \n",(int) ntohs(cli_addr.sin_port) );
        
   struct arpreq arp_req;
   bzero(&arp_req, sizeof(struct arpreq));
   int s;
   s= socket(AF_INET,SOCK_DGRAM,0);

   n=ioctl(s, SIOCGARP,(caddr_t) &arp_req);
   if(n<0)
    perror("ioctl");

   unsigned char *ptr = &arp_req.arp_ha.sa_data[0];
   printf("MAC: %X:%X:%X:%X:%X:%X \n",*ptr, *(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));

    close(mysock);
    close(n);	
 }
    
} while(1);

  return 0;
}
