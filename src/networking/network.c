#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <ncurses.h>

#include "network.h"
#include "errors.h"

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

void userJoin(char *name)
{
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

    userJoin(name);

    return sockfd;
}

struct Message *newMessage(int prefixLen, int commandLen, int trailingLen)
{
    struct Message *message = malloc(sizeof(struct Message));
    message->prefix   = malloc(prefixLen);
    message->command  = malloc(commandLen);
    message->trailing = malloc(trailingLen);
    return message;
}

void deleteMessage(struct Message **message)
{
    free((*message)->prefix);
    free((*message)->command);
    free((*message)->trailing);
    free( *message);
    *message = NULL;
}

struct Message *ircParse(char *str)
{
#ifdef DEBUG
    fprintf(Log, "%s", str);
#endif
    char *p;
    while ((p = strchr(str, '\r'))) {
        *p = ' ';
    }

    char *prefixEnd     = str + 1; //parsing prefix
    int prefixLen       = 0;
    if (str[0] == ':') {
        prefixEnd       = strchr(str, ' ') - 1;
        prefixLen       = prefixEnd - str - 1;
    }

    char *trailingStart = strchr(prefixEnd + 2, ' ') + 1; //parsing trailing
    int   trailingLen   = strlen(trailingStart) - 1;

    char *commandStart  = prefixEnd + 2; //parsing command
    char *commandEnd    = trailingStart - 2;
    int   commandLen    = commandEnd - commandStart + 1;

    struct Message *message = newMessage(prefixLen   + 1,
                                         commandLen  + 1,
                                         trailingLen + 1);

    strncpy(message->prefix, str+1, prefixLen);
    strncpy(message->command, commandStart, commandLen);
    strncpy(message->trailing, trailingStart, trailingLen);

    message->prefix[prefixLen]     = '\0';
    message->command[commandLen]   = '\0';
    message->trailing[trailingLen] = '\0';

    return message;
}

#ifdef DEBUG
void handlerError(char *command, int errorCode)
{
    if (errorCode != NO_ERROR) {
        fprintf(Log, "Handler for %s exited with error code %d\n",
                command, errorCode);
        exit(errorCode);
    }
}
#endif

struct Privmsg *newPrivmsg(struct Message *message)
{
    struct Privmsg *privmsg = malloc(sizeof(struct Privmsg));
    char *exclamationMark = strchr(message->prefix, '!');
    if (exclamationMark == NULL) {
        privmsg->nick  = malloc(strlen(message->prefix) + 1);
        privmsg->ident = NULL;
        privmsg->host  = NULL;
        strcpy(privmsg->nick, message->prefix);
    } else {
        int nickSize = exclamationMark - message->prefix;
        privmsg->nick = malloc(nickSize + 1);
        strncpy(privmsg->nick, message->prefix, nickSize);
        privmsg->nick[nickSize] = '\0';

        char *at = strchr(exclamationMark, '@');
        int identSize = at - exclamationMark - 1;
        privmsg->ident = malloc(identSize + 1);
        strncpy(privmsg->ident, exclamationMark + 1, identSize);
        privmsg->ident[identSize] = '\0';

        privmsg->host = malloc(strlen(at + 1) + 1);
        strcpy(privmsg->host, at + 1);
    }

    char *space = strchr(message->trailing, ' ');
    int targetSize = space - message->trailing + 1;
    privmsg->target = malloc(targetSize);
    strncpy(privmsg->target, message->trailing, targetSize);
    privmsg->target[targetSize - 1] = '\0';

    int messageLen = strlen(space + 2);
    privmsg->message = malloc(messageLen);
    strncpy(privmsg->message, space + 2, messageLen - 1);
    privmsg->message[messageLen - 1] = '\0';

    return privmsg;
}

int deletePrivmsg(struct Privmsg **privmsg)
{
    if (privmsg == NULL)
        return POINTER_NULL;

    if ((*privmsg)->nick    != NULL)
        free((*privmsg)->nick);
    if ((*privmsg)->ident   != NULL)
        free((*privmsg)->ident);
    if ((*privmsg)->host    != NULL)
        free((*privmsg)->host);
    if ((*privmsg)->target  != NULL)
        free((*privmsg)->target);
    if ((*privmsg)->message != NULL)
        free((*privmsg)->message);

    free(*privmsg);
    *privmsg = NULL;

    return NO_ERROR;
}

void handleMessage(struct Message *message, struct Handlers *handlers)
{
    int ret = NO_ERROR;
    if (strcmp(message->command, "PRIVMSG") == 0) {
        struct Privmsg *privmsg = newPrivmsg(message);
        ret = handlers->onPRIVMSG(privmsg);
        deletePrivmsg(&privmsg);
    } else if (strcmp(message->command, "JOIN") == 0) {
        ret = handlers->onJOIN(message);
    } else if (strcmp(message->command, "QUIT") == 0) {
        ret = handlers->onQUIT(message);
    } else if (strcmp(message->command, "NOTICE") == 0) {
        ret = handlers->onNOTICE(message);
    } else if (strcmp(message->command, "PING") == 0) {
        char pong[BUF_SIZE];
        sprintf(pong, "PONG: %s\n", message->trailing);
        write(sockfd, pong, strlen(pong));
    } else if (strcmp(message->command, "433") == 0) {
        char *space = strchr(message->trailing + 2, ' ');
        int len     = space - message->trailing - 2;
        char *name  = malloc(len + 2);
        strncpy(name, message->trailing + 2, len);
        name[len] = '_';
        userJoin(name);
        free(name);
    } else {
    }

#ifdef DEBUG
    handlerError(message->command, ret);
#endif
}

void *readFromSocket(void *handlers)
{
    handlers = (struct Handlers*)handlers;
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
                struct Message *message = ircParse(line);
                handleMessage(message, handlers);
                deleteMessage(&message);
                j = -1;
            }
        }
    }
    return NULL;
}

void readFromKeyboard(void *window)
{
    char buffer[BUF_SIZE];
    while(1) {
        int n = wscanw(window, "%[^\n]", buffer);
        sendIrcMessage(buffer);
    }
}

