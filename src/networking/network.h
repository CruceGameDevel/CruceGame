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
    int (*onPRIVMSG)(struct Message *);
    int (*onJOIN)(struct Message *);
    int (*onQUIT)(struct Message *);
};

int Connect(char *name);
void disconnect(int sockfd);
void sendIrcMessage(char *message);
void *readFromSocket(void *handlers);
void *readFromKeyboard(void *arg);
struct Message *ircParse(char *str);
void handleMessage(struct Message *message, struct Handlers *handlers);

#endif
