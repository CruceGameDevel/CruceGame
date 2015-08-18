#ifndef IRC_HANDLERS_H
#define IRC_HANDLERS_H

#include <ncurses.h>
#include <network.h>
#include "../cli/cli.h"


int onPRIVMSG_handler(struct Privmsg *privmsg);
int onJOIN_handler(struct Message *message);
int onQUIT_handler(struct Message *message);
int onNOTICE_handler(struct Message *message);

#endif
