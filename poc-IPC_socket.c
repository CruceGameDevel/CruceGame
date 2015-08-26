#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>



int openLocalhostSocket() {

    int serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSockfd < 0) {
        fprintf(stderr, "server socket creation failed\n");
    }

    int optval = 1;
    setsockopt(serverSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (serverSockfd < 0)
        fprintf(stderr, "setsockopt failed %d\n", errno);

    struct sockaddr_in serv_addr, cli_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);
    int bindval = bind(serverSockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (bindval < 0)
        fprintf(stderr, "Bind failed %d\n", errno);

    listen(serverSockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(serverSockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
        fprintf(stderr, "Accept failed %d\n", newsockfd);
    }


    return newsockfd;
}

int network_connect(char *hostname, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        fprintf(stderr, "Client Socket creation failed\n");

    struct hostent *server = gethostbyname(hostname);
    if(server == NULL) {
        fprintf(stderr, "Gethostbyname failed\n");
    } 

    struct sockaddr_in serv_addr;

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    memmove((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, 
        server->h_length);

    serv_addr.sin_port = htons(port);
 	
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Connect failed\n");
    }

    return sockfd;

}

int main() {
    int pid = fork();

    if (pid == 0) {
        printf("opening\n");
        int newsockfd = openLocalhostSocket();

        write(newsockfd, "test", 5);

        char buffer[10];
        read(newsockfd, buffer, 10);
        printf("SERVER BUFFER %s\n", buffer);

        printf("ended here\n");
        exit(EXIT_SUCCESS);
    }

    int sockfd = network_connect("localhost", 8080);

    char buffer[10];
    int out = read(sockfd, buffer, 10);
    printf("CLIENT BUFFER %s\n", buffer);
    write(sockfd, "check", 6);
    printf("%d", out);
    return 0;

}

