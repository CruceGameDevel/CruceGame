#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>

#include <pthread.h>
#include <ncurses.h>

#define CHANNEL "#cruce-devel "
#define PORT 6667
#define HOST_NAME "irc.freenode.net"

#define BUF_SIZE 64

static int sockfd;

void sendIrcMessage(char *message)   //TODO: errors
{
    char messageCommand[strlen(message) + strlen(CHANNEL) + 11];
    sprintf(messageCommand, "PRIVMSG %s:%s\n", CHANNEL, message);
    write(sockfd, messageCommand, strlen(messageCommand));
}

void disconnect(int sockfd)
{
    write(sockfd, "QUIT\n", 5);
    close(sockfd);
}

int Connect(char *name)
{
    Log = fopen("Log.txt", "w");
    int portno = PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
    bcopy((char*)server->h_addr_list[0], (char*)&serv_addr.sin_addr.s_addr, server->h_length);
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

void* readFromSocket(void *arg)
{
    char buffer[BUF_SIZE];
    int i = 0;
    while(1) {
        int n = read(sockfd, buffer, BUF_SIZE);
        char *p;
        while (p = strchr(buffer, '\r')) {
            *p = ' ';
        }
        buffer[n] = '\0';
        wprintw(arg, "%s", buffer);
        wrefresh(arg);
    }
    return NULL;
}

void *readFromKeyboard(void *arg)
{
    char buffer[BUF_SIZE];
    while(1) {
        int n = wscanw(arg, "%[^\n]", buffer);
        sendIrcMessage(buffer, strlen(buffer));
    }
    return NULL;
}

