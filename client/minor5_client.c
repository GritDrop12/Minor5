#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[256];

   if(argc < 2)
   {
	  printf("For this program please use command line argument.\n");
	  printf("Usage would be: ./a.out quit|get|put filename\n");
	  return 0;
   }

   portno = 3659;

   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

   server = gethostbyname("cse06.cse.unt.edu");

   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);

   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   if(argv[1] == "quit")
   {
	  n = write(sockfd, argv[1], strlen(buffer));
	  return 0;
   }

   if(argv[1] == "put")
   {
	  n = write(sockfd, argv[1], strlen(argv[1]));
	  n = write(sockfd, argv[2], strlen(buffer));
	  FILE *zp = fopen(argv[2],"r");

	  while(1)
	  {
		  unsigned char ruff[256] = {0};
		  int reads = fread(ruff,1,256,zp);

		  if(reads > 0)
		  {
			  n = write(sockfd, ruff, reads);
		  }
		  if(reads < 256)
		  {
			  if(feof(zp))
			  {
				  break;
			  }
		  }
	  }
   }
   char recbuff[256];
   if(argv[1]=="get")
   {
       n = write(sockfd,argv[1],strlen(buffer));
       n = write(sockfd, argv[2], strlen(buffer));
       FILE *zp = fopen(argv[2],"w");
       int bytesrec;
       while((bytesrec = read(sockfd,recbuff,256)>0))
       {
           fwrite(recbuff,1,bytesrec,zp);
       }
   }

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }

   /* Now read server response */
   //bzero(buffer,256);
   //n = read(sockfd, buffer, 255);

   //if (n < 0) {
   //   perror("ERROR reading from socket");
   //   exit(1);
   //}

   //printf("%s\n",buffer);
   return 0;
}
