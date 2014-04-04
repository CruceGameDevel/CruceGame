#include "cli.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARDS_PER_LINE 8
#define MAX_NAME_SIZE 50
#define ROUND_DIALOG_SCORE_SIZE 5

#define HIGHLIGHT_ATTRIBUTE A_BLINK

void welcomeMessage()
{
    printw("Welcome to a new game of Cruce\n\n");
}

int printCard(struct Card *card, int position, int selected, WINDOW *win)
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

    char upLeftCorner[]        = {0xE2, 0x95, 0xAD, 0x00};
    char upRightCorner[]       = {0xE2, 0x95, 0xAE, 0x00};
    char downLeftCorner[]      = {0xE2, 0x95, 0xB0, 0x00};
    char downRightCorner[]     = {0xE2, 0x95, 0xAF, 0x00};
    char horizontalLine[]      = {0xE2, 0x94, 0x80, 0x00};
    char verticalLine[]        = {0xE2, 0x94, 0x82, 0x00};
    
    int x, y;
    getyx(win, y, x);
    wprintw(win, "  %d  ", position + 1);
    wmove(win, y + 1, x);

    if (selected)
        wattron(win, HIGHLIGHT_ATTRIBUTE);
    wprintw(win, "%s%s%s%s%s%s", upLeftCorner, horizontalLine, horizontalLine,
             horizontalLine, horizontalLine, upRightCorner);
    wmove(win, y + 2, x);
    wprintw(win, "%s%c   %s", verticalLine, value, verticalLine);
    wmove(win, y + 3, x);
    wprintw(win, "%s%s   %s", verticalLine, suit, verticalLine);
    wmove(win, y + 4, x);
    wprintw(win, "%s    %s", verticalLine, verticalLine);
    wmove(win, y + 5, x);
    wprintw(win, "%s  %s %s", verticalLine, suit, verticalLine);
    wmove(win, y + 6, x);
    wprintw(win, "%s   %c%s", verticalLine, value, verticalLine);
    wmove(win, y + 7, x);
    wprintw(win, "%s%s%s%s%s%s",downLeftCorner, horizontalLine,horizontalLine, 
            horizontalLine, horizontalLine, downRightCorner);
    if (selected)
        wattroff(win, HIGHLIGHT_ATTRIBUTE);
    wmove(win, y + 9, x);
    wmove(win, y, x + 6);
    wrefresh(win);

    return NO_ERROR;
}

int printPlayerCards(struct Game *game, struct Player *player, int selected,
                     WINDOW *win)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (win == NULL)
        return POINTER_NULL;

    wprintw(win, "Your cards are:\n");

    int handId = 0;
    while(game->round->hands[handId]){
        handId++;
    }
    if (handId > 0)
        handId--;

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] != NULL) {
            if (!game_checkCard(player, game, game->round->hands[handId], i)) {
                wattron(win, COLOR_PAIR(1));
                printCard(player->hand[i], i, i==selected, win);
                wattroff(win, COLOR_PAIR(1));
            } else {
                printCard(player->hand[i], i, i==selected, win);
            }
        }
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

    int y, x;
    getyx(stdscr, y, x);

    WINDOW *cardsOnTableWindow = newwin(10, 79, y, 0);
#ifdef BORDERS
    box(cardsOnTableWindow, 0, 0);
#endif
    wprintw(cardsOnTableWindow, "The cards on table: \n");
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->cards[i] != NULL)
            printCard(hand->cards[i], i, 0, cardsOnTableWindow);
    wrefresh(cardsOnTableWindow);
    delwin(cardsOnTableWindow);

    refresh();

    WINDOW *cardsInHandWindow = newwin(10, 79, y + 10, 0); //MAGIC NUMBERS
#ifdef BORDERS
    box(cardsInHandWindow, 0, 0);
#endif
    keypad(cardsInHandWindow, TRUE);
    int ch, selected;
    if (game_checkCard(player, game, hand, 0) == 1)
        selected = 0;
    else
        selected = game_findNextAllowedCard(player, game, hand, 0);
    printPlayerCards(game, player, selected, cardsInHandWindow);
    while (( ch = wgetch(cardsInHandWindow)) != '\n') {
        wprintw(cardsInHandWindow, "%d", ch);
        switch (ch) {
            case KEY_LEFT:
                selected = game_findPreviousAllowedCard(player, game, hand,
                                                        selected);
                break;
            case KEY_RIGHT:
                selected = game_findNextAllowedCard(player, game, hand,
                                                    selected);
                break;
            case 'q':
                exit(0);
        }
        wclear(cardsInHandWindow);
        printPlayerCards(game, player, selected, cardsInHandWindow);
        wrefresh(cardsInHandWindow);
    }

    delwin(cardsInHandWindow);

    move(y + 20, 0);
    if (handId == 0 && playerId == 0)
        game->round->trump=player->hand[selected]->suit;

    round_putCard(player, selected, handId, game->round);

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

    printPlayerCards(game, game->round->players[playerId], -1, stdscr);

    int y, x;
    getyx(stdscr, y, x);
    move(y + 8, 0);

    printw("Insert a bid please: ");
    char ch = getch();
    while (round_placeBid(game->round->players[playerId], ch - '0', 
                          game->round)) { 
        printw("\nInsert a valid bid: ");
        ch = getch();
    }

    printw("\n\n");

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

int processingScore(char *score)
{
#ifdef DEBUG
    return atoi(score);
#else
    if (strcmp(score, "11") == 0)
        return 11;
    if (strcmp(score, "15") == 0)
        return 15;
    if (strcmp(score, "21") == 0)
        return 21;

    return -1;
#endif
}

int getScoreLimit()
{
    char score[3];

    printw("Insert the score limit (11-15-21): ");
    scanw("%s", score);

    while (processingScore(score) == -1) {
        printw("Insert the correct score limit (11-15-21): ");
        scanw("%s", score);
    }
    printw("\n");

    return processingScore(score);
}

/**
 * @brief Returns the length of the biggest name of a player in a round,
 *      helper for printRoundTerminationMessage.
 *
 * @param terminatedRound The round that has just finished. 
 *
 * @return The length of the biggest name of a player.
 */
int getBiggestNameSize(struct Round *terminatedRound)
{
    int maxNameSize = strlen(terminatedRound->players[0]->name);
    for(int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if(terminatedRound->players[i] != NULL) {
            if(strlen(terminatedRound->players[i]->name) > maxNameSize) {
                maxNameSize = strlen(terminatedRound->players[i]->name);
            }
        }
    }

    return maxNameSize;
}

int printRoundTerminationMessage(struct Round *terminatedRound, int *oldScore)
{
    if(terminatedRound == NULL || terminatedRound->players == NULL)
        return ROUND_NULL;

    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);

    int colorPair;
    int score;
    int playersNameWidth;  // used for alignmet
    int scoreLineSize = getBiggestNameSize(terminatedRound) + ROUND_DIALOG_SCORE_SIZE;

    printw("  _____                       _        _     _ \n"     
             " / ____|                     | |      | |   | |     \n"
            " | (___   ___ ___  _ __ ___  | |_ __ _| |__ | | ___ \n"
             "  \\___ \\ / __/ _ \\| '__/ _ \\ | __/ _` | '_ \\| |/ _ \\\n"
             "  ____) | (_| (_) | | |  __/ | || (_| | |_) | |  __/\n"
            " |_____/ \\___\\___/|_|  \\___|  \\__\\__,_|_.__/|_|\\___|\n\n\n");

    for(int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if(terminatedRound->players[i] != NULL) {
            printw("%s", terminatedRound->players[i]->name); 
            playersNameWidth = strlen(terminatedRound->players[i]->name);

            score = terminatedRound->players[i]->score - oldScore[i];         
            colorPair = (score > 0) ? 2 : 1;

            attron(COLOR_PAIR(colorPair));
            printw("%+*d \n", scoreLineSize - playersNameWidth, score);
            attroff(COLOR_PAIR(colorPair));
         
        }
    }
    return NO_ERROR;
}

