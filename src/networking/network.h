int Connect(char *name);
void disconnect(int sockfd);
void sendIrcMessage(int sockfd, char *message, int len);
void *readFromSocket(void *arg);
void *readFromKeyboard(void *arg);
