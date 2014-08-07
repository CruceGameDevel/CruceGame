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

#include "network.h"

#define CHANNEL "#cruce-devel "
#define PORT 6667
#define HOST_NAME "irc.freenode.net"

#define BUF_SIZE 1024

static int sockfd;

#ifdef DEBUG
FILE *Log;

void printString(char *s) {
    for (int i = 0; i < BUF_SIZE; i++) {
        if (s[i] > ' ')
            fprintf(Log, "%c ", s[i]);
        else
            fprintf(Log, "%d ", s[i]);
    }
    fprintf(Log, "\nEND\n");
}
#endif

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
#ifdef DEBUG
    Log = fopen("Log.txt", "w");
#endif
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

void *readFromSocket(void *arg)
{
    char buffer[BUF_SIZE];
    char line[BUF_SIZE];
    memset(buffer, 0, BUF_SIZE);
    int j = 0;
    while(1) {
        int n = read(sockfd, buffer, BUF_SIZE);
        buffer[n] = '\0';
        for (int i = 0; i < BUF_SIZE && buffer[i] != '\0'; i++, j++) {
            line[j] = buffer[i];
            if (buffer[i] == '\n') {
                line [j + 1] = '\0';
                ircParse(line, arg);
                j = -1;
            }
        }
    }
    return NULL;
}

void *readFromKeyboard(void *arg)
{
    char buffer[BUF_SIZE];
    while(1) {
        int n = wscanw(arg, "%[^\n]", buffer);
        sendIrcMessage(buffer);
    }
    return NULL;
}

void ircParse(char *str, void *arg)
{
    char *p;
    while (p = strchr(str, '\r')) {
        *p = ' ';
    }
    if (strncmp(str, "PING", 4) == 0) {
        char buffer[strlen(str)];
        strcpy(buffer, str);
        buffer[1] = 'O';
        write(sockfd, buffer, strlen(buffer));
    } else if (strstr(str, "PRIVMSG") != NULL) {
        //parse messages formatted ":sender!... PRIVMSG :message"
        char sender[30]; //TODO: constant
        int senderLen = strchr(str, ' ') - str;
        strncpy(sender, str, senderLen); //TODO: add exception for n>30
        sender[senderLen] = '\0';
        char *exclamationMark = strchr(sender, '!');
        if (exclamationMark) {
            *exclamationMark = '\0';
        }
        char *message = strchr(str + 1, ':');
        char buffer[strlen(message) + strlen(sender) + 11];
        sprintf(buffer, "%s: %s", sender, message + 1);
        wprintw(arg, "%s", buffer);
    } else {
        wprintw(arg, "%s", str);
    }
    wrefresh(arg);
}

