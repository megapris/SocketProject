#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1012];
    if (argc < 3) {
        printf("Not enough arguments\n");
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("Error opening socket\n");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("Error: no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        printf("Error connecting\n");
    int i = 0;
    while (i < 5) {
        printf("Please enter the message: ");
        bzero(buffer, 1012);
        fgets(buffer, 1011, stdin);
        n = write(sockfd, buffer, 1011);
        if (n < 0)
            printf("Error writing to socket");
        bzero(buffer, 1012);
        n = read(sockfd, buffer, 1011);
        if (n < 0)
            printf("Error reading from socket");
        printf("%s\n", buffer);
        i++;
    }
    close(sockfd);
    return 0;
}