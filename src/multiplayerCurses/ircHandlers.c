#include "ircHandlers.h"

int onPRIVMSG_handler(struct Privmsg *privmsg)
{
    if (privmsg->message[0] == GAME_COMMAND_FLAG) {
        wprintw(getPrintWin(), "GAME COMMAND\n");
    } else {
        wprintw(getPrintWin(), "%s %s\n", privmsg->nick, privmsg->message);
    }
    wrefresh(getPrintWin());
    return NO_ERROR;
}

int onJOIN_handler(struct Message *message)
{
    wprintw(getPrintWin(), "JOIN\n");
    wprintw(getPrintWin(), "prefix: %s\n", message->prefix);
    wprintw(getPrintWin(), "command: %s\n", message->command);
    wprintw(getPrintWin(), "trailing: %s\n", message->trailing);
    wrefresh(getPrintWin());
    return NO_ERROR;
}

int onQUIT_handler(struct Message *message)
{
    wprintw(getPrintWin(), "QUIT\n");
    wprintw(getPrintWin(), "prefix: %s\n", message->prefix);
    wprintw(getPrintWin(), "command: %s\n", message->command);
    wprintw(getPrintWin(), "trailing: %s\n", message->trailing);
    wrefresh(getPrintWin());
    return NO_ERROR;
}

int onNOTICE_handler(struct Message *message)
{
    wprintw(getPrintWin(), "NOTICE: %s\n", message->trailing);
    wrefresh(getPrintWin());
    return NO_ERROR;
}


