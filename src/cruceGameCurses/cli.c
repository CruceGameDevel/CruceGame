#include "cli.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>

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
    printw("  %d  ", position + 1);
    move(y + 1, x);
    printw(" ___ ");
    move(y + 2, x);
    printw("|%c  |", value);
    move(y + 3, x);
    printw("|%s  |", suit);
    move(y + 4, x);
    printw("|  %s|", suit);
    move(y + 5, x);
    printw("|  %c|", value);
    move(y + 6, x);
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

    struct Player *player = team_createPlayer(name, 1);
    free(name);
    return player;
}

int printScore(struct Game *game, struct Round *round)
{
    if (game == NULL)
        return GAME_NULL;
    if (round == NULL)
        return ROUND_NULL;

    char verticalBoxDouble[]           = {0xe2, 0x95, 0x91, 0x00};
    char horizontalBoxDouble[]         = {0xe2, 0x95, 0x90, 0x00};
    char downRightBoxDouble[]          = {0xe2, 0x95, 0x94, 0x00};
    char downLeftBoxDouble[]           = {0xe2, 0x95, 0x97, 0x00};
    char upRightBoxDouble[]            = {0xe2, 0x95, 0x9a, 0x00};
    char upLeftBoxDouble[]             = {0xe2, 0x95, 0x9d, 0x00};
    char downHorizontalBoxDouble[]     = {0xe2, 0x95, 0xa6, 0x00};
    char upHorizontalBoxDouble[]       = {0xe2, 0x95, 0xa9, 0x00};
    char verticalHorizontalBoxDouble[] = {0xe2, 0x95, 0xac, 0x00};
    char verticalRightBoxDouble[]      = {0xe2, 0x95, 0xa0, 0x00}; 
    char verticalLeftBoxDouble[]       = {0xe2, 0x95, 0xa3, 0x00};

    int maxLength = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (game->players[i] != NULL) {
            int length = strlen(game->players[i]->name);
            if (length > maxLength)
                maxLength = length;
        }
    maxLength++;

    if (maxLength < 4 )
        maxLength = 4; 

    int x, y;
    int line = 0;
    getyx(stdscr, y, x);

    printw("%s",downRightBoxDouble);
    for (int i = 1; i <= maxLength + 13; i++){
        if (i == maxLength + 1 || i == maxLength + 8)
            printw("%s", downHorizontalBoxDouble);
        else
            printw("%s", horizontalBoxDouble);
    }
    printw("%s", downLeftBoxDouble);
    line++;

    move(y + 1, x);
    printw("%sName", verticalBoxDouble);
    move(y + 1, x + maxLength + 1);
    printw("%sPoints", verticalBoxDouble);
    move(y + 1, x + maxLength + 8);
    printw("%sScore%s", verticalBoxDouble, verticalBoxDouble);
    line++;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            move(y + line, x);
            printw("%s", verticalRightBoxDouble);
            for (int j = 1; j <= maxLength + 13; j++) {
                if (j == maxLength + 1 || j == maxLength + 8)
                    printw("%s", verticalHorizontalBoxDouble);
                else
                    printw("%s", horizontalBoxDouble);
            }
            printw("%s", verticalLeftBoxDouble);
            line++;
            move(y + line, x);
            printw("%s%s ", verticalBoxDouble, round->players[i]->name);
            move(y + line, x + maxLength + 1);
            printw("%s %*d", verticalBoxDouble, 5, round->pointsNumber[i]);
            move(y + line, x + maxLength + 8);
            printw("%s %*d", verticalBoxDouble, 4, game->players[i]->score);
            move(y + line, x + maxLength + 14);
            printw("%s", verticalBoxDouble);
            line++;
        }            
    }

    move(y + line, x);
    printw("%s", upRightBoxDouble); 
    for (int i = 1; i <= maxLength + 13; i++) {
        if (i == maxLength + 1 || i == maxLength + 8)
            printw("%s", upHorizontalBoxDouble);
        else
            printw("%s", horizontalBoxDouble);
    }
    printw("%s\n", upLeftBoxDouble);

    return NO_ERROR;
}

int pickCard (struct Player *player, struct Game *game, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (game == NULL)
        return GAME_NULL;
    if (hand == NULL)
        return HAND_NULL;

    printw("\nInsert a card number: ");
    char ch = getch();
    while (ch < '1' || ch > '8' || player->hand[ch - '1'] == NULL ||
           game_checkCard(player, game, hand, ch - '1') != 1) {
        printw("\nInsert a correct card number: ");
        ch = getch();
    }

    return ch - '1';
}

void createEmptyTeams(struct Game *game)
{
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            struct Team *team = team_createTeam(game->players[i]->name);
            team_addPlayer(team, game->players[i]);
            game_addTeam(team, game);
        }
    }
}


int formTeams (struct Game *game)
{
    if (game == NULL)
        return GAME_NULL;
    if (game->numberPlayers == 0)
        return GAME_EMPTY;
    if (game->numberPlayers == 1)
        return INSUFFICIENT_PLAYERS;

    if (game->numberPlayers < 4) {
        createEmptyTeams(game);
        return NO_ERROR;
    }

    printw("Do you want to play on teams? (Y/n) ");
    char ch = getch();
    printw("\n");

    if (ch == 'n' || ch == 'N') {
        createEmptyTeams(game);
        return NO_ERROR;
    }

    printw("Player 1 (%s): Please insert your teammate's id: ",
            game->players[0]->name);
    ch = getch();
    while (ch < '2' || ch > '4') {
        printw("\nPlease insert a correct player id. ");
        ch = getch();
    }
    printw("\n");
    ch--;

    struct Player* backup = game->players[2];
    game->players[2]      = game->players[ch-'0'];
    game->players[ch-'0'] = backup;

    char *numerals[] = {"first", "second"};
    for (int i = 0; i < 2; i++) {
        char *teamName = malloc(100); //WARNING: MAGIC CONSTANT
        printw("Insert %s team's name: ", numerals[i]);
        scanw("%s", teamName);

        struct Team *team = team_createTeam(teamName);
        free(teamName);    

        team_addPlayer(team, game->players[i]);
        team_addPlayer(team, game->players[i + 1]);
        game_addTeam(team, game);
    }

    return NO_ERROR;
}

int displayCardsAndPickCard(struct Game *game, int playerId)
{
    if (game == NULL)
        return GAME_NULL;
    int handId=0;
    while(handId < MAX_HANDS && game->round->hands[handId] != NULL)
            handId++;
    handId--;
    if (game->round->hands[handId] == NULL)
        return HAND_NULL;

    struct Hand *hand = game->round->hands[handId];
    struct Player *player = hand->players[playerId];

    char suit[] = {0xE2, 0x99, 0x00, 0x00};
    switch (game->round->trump) {
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
            break;
    }

    printw("Player %d %s\n", playerId + 1, player->name);

    if (game->round->trump != SuitEnd)
        printw("The trump is: %s\n", suit);
    else
        printw("The trump was not set.\n");

    printw("The cards on table: ");
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->cards[i] != NULL)
            printCard(hand->cards[i], i);

    int y, x;
    getyx(stdscr, y, x);
    move(y + 7, 0);

    printw("Your cards: ");
    printPlayerCards(player);

    move(y + 14, 0);
    int cardId = pickCard(player, game, hand);

    if (handId == 0 && playerId == 0)
        game->round->trump=player->hand[cardId]->suit;

    round_putCard(player, cardId, handId, game->round);

    return NO_ERROR;
}

int getBid(struct Game *game, int playerId)
{
    if (game == NULL)
        return GAME_NULL;
    if (playerId < 0 || playerId >= MAX_GAME_PLAYERS)
        return ILLEGAL_VALUE;
    if (game->round == NULL)
        return ROUND_NULL;
    if (game->round->players[playerId] == NULL)
        return PLAYER_NULL;

    printw("Player %d %s\n", playerId + 1,
                             game->round->players[playerId]->name);

    printw("Your cards are:\n");

    printPlayerCards(game->round->players[playerId]);

    int y, x;
    getyx(stdscr, y, x);
    move(y + 7, 0);

    printw("Insert a bid please: ");
    char ch = getch();
    while (round_placeBid(game->round->players[playerId], ch - '0', 
                          game->round)) { 
        printw("\nInsert a valid bid: ");
        ch = getch();
    }

    printw("\n");

    return NO_ERROR;
}

int displayWinner(struct Team *winner) {
    if(winner->name != NULL) { 
        printw("The winner of the game is %s\n", winner->name);
        return NO_ERROR; 
    } else {
        return POINTER_NULL;
    }
}

