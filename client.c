#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server; 
    /*The hostent structure is used by functions to store information about a given host, such as host name, IPv4 address, and so forth*/
    char buffer[256];
    char buffer2[1012];

    char* hugestring = malloc(1012);



    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    /* gethostname() system call returns a null-terminated hostname*/

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    // The bzero() function erases the data in the n bytes of the memory
    // starting at the location pointed to by s, by writing zeros (bytes
    // containing '\0') to that area
    // void bzero(void *s, size_t n); 
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    // The bcopy() function copies n bytes from src to dest.  The result
    // is correct, even when both areas overlap.
    // void bcopy(const void *src, void *dest, size_t n);

    printf("this is the serverLength: %d\n",server->h_length);
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Please enter the message: ");

    bzero(buffer2, 1012);
    fgets(buffer2, 1011, stdin);
    // scanf("%c",buffer2);


    // strcpy(buffer2,"Task: Extend the following progr implement seß able to send and receive messages of up to 1000 characters back and forth any number of times without quitting. Deliverables:Task: Extend the following programs to implement seamless chat application between client and server processes. Client and server should be able to send and receive messages of up to 1000 characters back and forth any number of times without quitting. Deliverables:Task: Extend the following programs to implement seamless chat application between client and server processes. Client and server should be able to send and receive messages of up to 1000 characters back and forth any number of times without quitting. Deliverables:");
    
    printf("buffer2 is: %s and its length: %lu\n",buffer2,strlen(buffer2));
    delay(1000);
    // write() writes up to count bytes from the buffer starting at buf
    // to the file referred to by the file descriptor fd.

    // The number of bytes written may be less than count if, for
    // example, there is insufficient space on the underlying physical
    // medium, or the RLIMIT_FSIZE resource limit is encountered (see
    // setrlimit(2)), or the call was interrupted by a signal handler
    // after having written less than count bytes.  (See also pipe(7).)
    // ssize_t write(int fd, const void *buf, size_t count);
    n = write(sockfd, buffer2, 1011);

    if (n < 0) 
            error("ERROR writing to socket");

    bzero(buffer2, 1012);


    // read() attempts to read up to count bytes from file descriptor fd
    // into the buffer starting at buf.
    // ssize_t read(int fd, void *buf, size_t count);
    n = read(sockfd, buffer2, 1011);

    if (n < 0) 
            error("ERROR reading from socket");

    printf("%s\n",buffer2);

    return 0;
}
