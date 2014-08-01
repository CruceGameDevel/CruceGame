int Connect(char *name);
void disconnect(int sockfd);
void sendIrcMessage(char *message);
void *readFromSocket(void *arg);
void *readFromKeyboard(void *arg);
void ircParse(char *str, void *);
