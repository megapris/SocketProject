/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[270];
     char buffer2[1012];

     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        error("ERROR opening socket");

     // The bzero() function erases the data in the n bytes of the memory
     // starting at the location pointed to by s, by writing zeros (bytes
     // containing '\0') to that area
     // void bzero(void *s, size_t n); 
     bzero((char *) &serv_addr, sizeof(serv_addr)); 
	/* erases the data in the n bytes of the memory*/
 
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	/* Host to net byte order for short int*/

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);
	/* 5 outstanding connections*/

     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);

     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer2,1012);


     // read() attempts to read up to count bytes from file descriptor fd
     // into the buffer starting at buf.
     // ssize_t read(int fd, void *buf, size_t count);
     n = read(newsockfd, buffer2, 1011);

     if (n < 0) error("ERROR reading from socket");

    
    
     printf("Here is the message: %s\n",buffer2);

     // write() writes up to count bytes from the buffer starting at buf
     // to the file referred to by the file descriptor fd.

     // The number of bytes written may be less than count if, for
     // example, there is insufficient space on the underlying physical
     // medium, or the RLIMIT_FSIZE resource limit is encountered (see
     // setrlimit(2)), or the call was interrupted by a signal handler
     // after having written less than count bytes.  (See also pipe(7).)
     // ssize_t write(int fd, const void *buf, size_t count);

     n = write(newsockfd, "I got your message", 18);

     if (n < 0) error("ERROR writing to socket");

     return 0; 
}
