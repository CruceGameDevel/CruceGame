#ifndef NETWORK_H
#define NETWORK_H

struct Message{
    char *prefix;
    char *command;
    char *trailing;
};

struct Privmsg{
    char *nick;
    char *ident;
    char *host;
    char *target;
    char *message;
};

struct Handlers{
    int (*onPRIVMSG)(struct Privmsg *);
    int (*onJOIN)(struct Message *);
    int (*onQUIT)(struct Message *);
    int (*onNOTICE)(struct Message *);
};

int Connect(char *name);
void disconnect(int sockfd);
void sendIrcMessage(char *message);
void *readFromSocket(void *handlers);
void readFromKeyboard(void *window);

#endif
