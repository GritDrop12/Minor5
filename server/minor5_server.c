#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 3659;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   printf("Waiting for connections\n");
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }

   /* If connection is established then start communicating */

   n = read( newsockfd,buffer,255 );
   printf("%s\n",buffer);
   if(buffer == "quit")
   {
       return 0;
   }
   else if(buffer == "put")
   {
       n = read(newsockfd,buffer,256);
       FILE *xp;
       xp = fopen(buffer,"w");
       int bytesrec;
       char recbuff[256];
       while((bytesrec = read(newsockfd,recbuff,256))>0)
       {
           fwrite(recbuff,1,bytesrec,xp);
       }

   }
   else if(buffer == "get")
   {
       n = read(newsockfd,buffer,256);
       FILE *xp;
       xp = fopen(buffer,"r");

       while(1)
       {
           unsigned char rruf[256] = {0};
           int cre = fread(rruf,1,256,xp);

           if( cre > 0)
           {
               write(newsockfd,rruf,cre);
           }

           if( cre < 256)
           {
               if(feof(xp))
               {
                   break;
               }
           }
       }
   }
   else
   {
       return 0;
   }
   //n = write(newsockfd,"I got your message",18);

   /*if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }*/

   return 0;
}
