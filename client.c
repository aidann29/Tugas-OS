#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void error_read(int a)
{
    if (a < 0) error("ERROR reading from socket");
}

void error_write(int a)
{
     if (a < 0) error("ERROR writing to socket");
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, i, batas1, batas2;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    int buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    printf("Program ini akan menampilkan bilangan prima\n");
    printf("dengan batas bawah dan batas atas sesuai dengan input user.\n\n");
    printf("Batas1: ");
    bzero(buffer,256);
    scanf("%d", buffer);
    n = write(sockfd,buffer,255);
    error_write(n);
    batas1 = buffer[0];
    
    bzero(buffer,256);
    printf("Batas2: ");
    scanf("%d", buffer);
    n = write(sockfd,buffer,255);
    error_write(n);
    batas2 = buffer[0];
    
    printf ("Jumlah bilangan prima dari %d sampai %d adalah: ", batas1, batas2);
    
    n = read(sockfd,buffer,255);
    error_read(n);
    printf ("%d\n", buffer[0]);

    close(sockfd);
    return 0;
}
