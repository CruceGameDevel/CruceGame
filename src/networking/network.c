#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define CHANNEL "#cruce-devel "
#define BUF_SIZE 512
#define PORT 6667
#define HOST_NAME "irc.freenode.net"

void sendIrcMessage(int sockfd, char *message)   //TODO: errors
{
    char messageCommand[BUF_SIZE + 9];
    sprintf(messageCommand, "PRIVMSG %s:%s", CHANNEL, message);
    write(sockfd, messageCommand, strlen(messageCommand));
}

void disconnect(int sockfd)
{
    write(sockfd, "QUIT\n", 5);
    close(sockfd);
}

int Connect(char *name)
{
    int portno = PORT;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;     //TODO: error code
    }
    struct hostent *server = gethostbyname(HOST_NAME);
    if (server == NULL) {
        return -2;     //TODO: error code
    }
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        return -3; //TODO: error code
    }

    char nickCommand[20];
    char joinCommand[20];
    char userCommand[20];

    sprintf(nickCommand, "NICK %s\n", name);
    sprintf(userCommand, "USER %s 8 * :%s\n", name, name);
    sprintf(joinCommand, "JOIN %s\n", CHANNEL);

    write(sockfd, "PASS *\n", 7);
    write(sockfd, nickCommand, strlen(nickCommand));
    write(sockfd, userCommand, strlen(userCommand));
    write(sockfd, joinCommand, strlen(joinCommand));

    return sockfd;
}

void reset_timeout(struct timeval *timeout)
{
    timeout->tv_sec = 250;
    timeout->tv_usec = 0;     //TODO: use constants
}

void loop(int infd, int outfd)
{
    fd_set master_set, working_set;
    FD_ZERO(&master_set);
    FD_ZERO(&working_set);
    FD_SET(infd, &master_set);
    FD_SET(outfd, &master_set);

    char buffer[BUF_SIZE];
    struct timeval timeout;
    int rc=1;
    while (rc > 0){
        reset_timeout(&timeout);
        memcpy(&working_set, &master_set, sizeof(master_set));
        rc = select(outfd + 1, &working_set, NULL, NULL, &timeout);
        if (FD_ISSET(infd, &working_set)) {
            int n = read(infd, buffer, BUF_SIZE);
            buffer[n] = '\0';
            sendIrcMessage(outfd, buffer);
        }
        if (FD_ISSET(outfd, &working_set)) {
            int n = read(outfd, buffer, BUF_SIZE);
            buffer[n] = '\0';
            printf("%s", buffer);
        }
    }

    printf("\n TIMEOUT DISCONNECT");
    disconnect(outfd);
}

int main() {
    int sockfd = Connect("Dipol");
    loop(0, sockfd);
    return 0;
}


