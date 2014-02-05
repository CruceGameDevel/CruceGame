#include "cli.h"

#include <curses.h>
#include <stdlib.h>

#define MAX_CARDS_PER_LINE 8
#define MAX_NAME_SIZE 50

void welcomeMessage()
{
    printw("Welcome to a new game of Cruce\n\n");
}

int printCard(struct Card *card, int position)
{
    char suit[] = {0xE2, 0x99, 0x00, 0x00};
    switch (card->suit) {
        case DIAMONDS:
            suit[2] = 0xA6;
            break;
        case CLUBS:
            suit[2] = 0xA3;
            break;
        case HEARTS:
            suit[2] = 0xA5;
            break;
        case SPADES:
            suit[2] = 0xA0;
            break;
        default:
            return ILLEGAL_VALUE;
    }

    char value;
    switch (card->value) {
        case 0:
            value = '9';
            break;
        case 2:
        case 3:
        case 4:
            value = '0' + card->value;
            break;
        case 10:
            value = 'X';
            break;
        case 11:
            value = 'A';
            break;
        default:
            return ILLEGAL_VALUE;
    }

    int x, y;
    getyx(stdscr, y, x);
    printw(" ___ ");
    move(y+1, x);
    printw("|%c  |", value);
    move(y+2, x);
    printw("|%s  |", suit);
    move(y+3, x);
    printw("|  %s|", suit);
    move(y+4, x);
    printw("|  %c|", value);
    move(y+5, x);
    char bottom[] = {0xE2, 0x80, 0xBE, 0x00};
    printw(" %s%s%s ", bottom, bottom, bottom);
    move(y, x + 6);
    refresh();

    return NO_ERROR;
}

int printPlayerCards(struct Player *player)
{
    if (player == NULL)
        return PLAYER_NULL;

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] != NULL)
            printCard(player->hand[i], i);

    }
    return NO_ERROR;
}

int getNoOfPlayers()
{
    printw("Insert the number of players (2-4) ");
    char ch = getch();
    while (ch < '2' || ch > '4') {
        printw("\nPlease insert a number between 2 and 4. ");
        ch = getch();
    }
    printw("\n");

    return ch - '0';
}

struct Player *newPlayer(int i)
{
    char *name = malloc(MAX_NAME_SIZE*sizeof(char));
    printw("Insert player %d name: ", i);
    scanw("%s", name);

    struct Player *player = team_createPlayer(name, 0, 1);
    return player;
}
