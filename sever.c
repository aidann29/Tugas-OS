/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int batas1, batas2, batas3, i, j, flag, x, n, sockfd, newsockfd, portno, jumlah;
int buffer[256];

void mulai();
void hitungan();

void error(const char *msg)
{
    perror(msg);
    exit(1);
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
     socklen_t clilen;
     
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
          
     mulai();
     
     close(newsockfd);
     close(sockfd);
     return 0; 
}
  
void mulai()
{
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    error_read(n);
    batas1 = buffer[0];
    printf("Batas1 = %d\n", batas1);
    
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    error_read(n);
    batas2 = buffer[0];
    printf("Batas2 = %d\n", batas2);
    
    if (batas1 < batas2)
    {
        hitungan();
    }
    else if (batas1 > batas2)
    {
        batas3 = batas1;
        batas1 = batas2;
        batas2 = batas3;
        hitungan();
    }
}

void hitungan()
{
    if ( batas1 % 2 == 0)
    {
        batas1++;
    }

    for (i = batas1; i <= batas2; i++)
    {
        flag = 0;

        for (j = 2; j <= i / 2; j++)
        {
            if ((i % j) == 0)
            {
                flag = 1;
                break;
            }
        }

        if (i != 1)
        {
            if (flag == 0)
            {
                jumlah++;
            }   
        }
    }
    buffer[0] = jumlah;
    n = write(newsockfd, buffer, 255);
    printf ("Jumlah bil. prima = %d", jumlah);
}
