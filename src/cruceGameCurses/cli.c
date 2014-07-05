/**
 * @file cli.c
 * @brief Contains implementations of the functions used for creating
 *        the user interface. These functions are declared in the 
 *        header file cli.h.
 */

#include "cli.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#else
#include <unistd.h>
#endif

#define MAX_CARDS_PER_LINE 8
#define MAX_NAME_SIZE 20
#define ROUND_DIALOG_SCORE_SIZE 5
#define SLEEP_TIME 2

void welcomeMessage(WINDOW *win)
{
    wprintw(win,
            "  _____                        _____                           \n"
            " / ____|                      / ____|                          \n"
            "| |     _ __ _   _  ___ ___  | |  __  __ _ _ __ ___   ___      \n"
            "| |    | '__| | | |/ __/ _ \\ | | |_ |/ _` | '_ ` _ \\ / _ \\  \n"
            "| |____| |  | |_| | (_|  __/ | |__| | (_| | | | | | |  __/     \n"
            " \\_____|_|   \\__,_|\\___\\___|  \\_____|\\__,_|_| |_| |_|\\___|"
            "\n\n");
}

void gameEndingMessage(WINDOW *win, const struct Team *team)
{
    wprintw(win,
       " ______           _    ____   __    _____                           \n"
       "|  ____|         | |  / __ \\ / _|  / ____|                         \n"
       "| |__   _ __   __| | | |  | | |_  | |  __  __ _ _ __ ___   ___      \n"
       "|  __| | '_ \\ / _` | | |  | |  _| | | |_ |/ _` | '_ ` _ \\ / _ \\  \n"
       "| |____| | | | (_| | | |__| | |   | |__| | (_| | | | | | |  __/     \n"
       "|______|_| |_|\\__,_|  \\____/|_|    \\_____|\\__,_|_| |_| |_|\\___|\n"
       "\n\n");

    for (int i = MAX_TEAM_PLAYERS - 1; i >= 0; --i)
        if (team->players[i] != NULL) {
            if (i > 0) {
                wprintw(win, "%s, ", team->players[i]->name);
            } else {
                wprintw(win, "%s ", team->players[i]->name);
            }
        }
    wprintw(win, "won the game.");
}

int printCard(WINDOW *win, const struct Card *card, const int frameColor)
{
    int colorPair;
    char suit[] = {0xE2, 0x99, 0x00, 0x00};
    switch (card->suit) {
        case DIAMONDS:
            colorPair = 3;
            suit[2] = 0xA6;
            break;
        case CLUBS:
            colorPair = 4;
            suit[2] = 0xA3;
            break;
        case HEARTS:
            colorPair = 1;
            suit[2] = 0xA5;
            break;
        case SPADES:
            colorPair = 2;
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

#ifdef WIN32
    char upLeftCorner[]        = {0xE2, 0x94, 0x8C, 0x00};
    char upRightCorner[]       = {0xE2, 0x94, 0x90, 0x00};
    char downLeftCorner[]      = {0xE2, 0x94, 0x94, 0x00};
    char downRightCorner[]     = {0xE2, 0x94, 0x98, 0x00};
    char horizontalLine[]      = {0xE2, 0x94, 0x80, 0x00};
    char verticalLine[]        = {0xE2, 0x94, 0x82, 0x00};
#else
    char upLeftCorner[]        = {0xE2, 0x95, 0xAD, 0x00};
    char upRightCorner[]       = {0xE2, 0x95, 0xAE, 0x00};
    char downLeftCorner[]      = {0xE2, 0x95, 0xB0, 0x00};
    char downRightCorner[]     = {0xE2, 0x95, 0xAF, 0x00};
    char horizontalLine[]      = {0xE2, 0x94, 0x80, 0x00};
    char verticalLine[]        = {0xE2, 0x94, 0x82, 0x00};
#endif

    int x, y;
    getyx(win, y, x);

    wattron(win, COLOR_PAIR(frameColor));
    wprintw(win, "%s%s%s%s%s%s", upLeftCorner, horizontalLine, horizontalLine,
             horizontalLine, horizontalLine, upRightCorner);

    wmove(win, y + 1, x);
    wprintw(win, "%s", verticalLine);
    wattroff(win, COLOR_PAIR(frameColor));
    wprintw(win, "%c   ", value);
    wattron(win, COLOR_PAIR(frameColor));
    wprintw(win, "%s", verticalLine);
    wmove(win, y + 2, x);

    wprintw(win, "%s", verticalLine);
    wattroff(win, COLOR_PAIR(frameColor));
    wattron(win, COLOR_PAIR(colorPair));
    wprintw(win, "%s", suit);
    wattroff(win, COLOR_PAIR(colorPair));
    wattron(win, COLOR_PAIR(frameColor));
    wprintw(win, "   %s", verticalLine);
    wmove(win, y + 3, x);

    wprintw(win, "%s    %s", verticalLine, verticalLine);
    wmove(win, y + 4, x);
    wprintw(win, "%s", verticalLine);
    wattroff(win, COLOR_PAIR(frameColor));
    wattron(win, COLOR_PAIR(colorPair));
    wprintw(win, "  %s", suit);
    wattroff(win, COLOR_PAIR(colorPair));

    wattron(win, COLOR_PAIR(frameColor));
    wprintw(win, " %s", verticalLine);
    wmove(win, y + 5, x);
    wprintw(win, "%s   ", verticalLine);
    wattroff(win, COLOR_PAIR(frameColor));
    wattroff(win, COLOR_PAIR(frameColor));
    wprintw(win, "%c", value);
    wattron(win, COLOR_PAIR(frameColor));
    wprintw(win, "%s", verticalLine);
    wmove(win, y + 6, x);

    wprintw(win, "%s%s%s%s%s%s",downLeftCorner, horizontalLine,horizontalLine, 
            horizontalLine, horizontalLine, downRightCorner);
    wattroff(win, COLOR_PAIR(frameColor));
    
    wmove(win, y + 9, x);
    wmove(win, y, x + 6);
    wrefresh(win);

    return NO_ERROR;
}

int printPlayerCards(const struct Game *game, struct Player *player, 
                     const int selected, WINDOW *win)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (win == NULL)
        return POINTER_NULL;

    wprintw(win, "Your cards:\n");

    int handId = 0;
    while(game->round->hands[handId]){
        handId++;
    }
    if (handId > 0)
        handId--;

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] != NULL) {
            if (game_checkCard(player, game, game->round->hands[handId], i)) {
                if (i == selected)
                    printCard(win, player->hand[i], 3);
                else
                    printCard(win, player->hand[i], 7);
            } else {
                printCard(win, player->hand[i], 1);
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

struct Player *newPlayer(const int i)
{
    char format[20]; //used to store the format string.
    char *name = malloc(MAX_NAME_SIZE*sizeof(char));
    int length;
    do{
        printw("Insert player %d name: ", i);
        sprintf(format, "%%%i[^\n]", MAX_NAME_SIZE);
        scanw(format, name);
        length = strlen(name);
    } while(length == 0);

    struct Player *player = team_createPlayer(name, 1);
    free(name);
    return player;
}

int printScore(const struct Game *game, const struct Round *round, WINDOW *win)
{
    if (game == NULL)
        return GAME_NULL;
    if (round == NULL)
        return ROUND_NULL;

    char verticalBox[]           = {0xe2, 0x94, 0x82, 0x00};
    char horizontalBox[]         = {0xe2, 0x94, 0x80, 0x00};
    char downRightBox[]          = {0xe2, 0x94, 0x8c, 0x00};
    char downLeftBox[]           = {0xe2, 0x94, 0x90, 0x00};
    char upRightBox[]            = {0xe2, 0x94, 0x94, 0x00};
    char upLeftBox[]             = {0xe2, 0x94, 0x98, 0x00};
    char downHorizontalBox[]     = {0xe2, 0x94, 0xac, 0x00};
    char upHorizontalBox[]       = {0xe2, 0x94, 0xb4, 0x00};
    char verticalHorizontalBox[] = {0xe2, 0x94, 0xbc, 0x00};
    char verticalRightBox[]      = {0xe2, 0x94, 0x9c, 0x00};
    char verticalLeftBox[]       = {0xe2, 0x94, 0xa4, 0x00};

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
    getyx(win, y, x);

    wprintw(win, "%s",downRightBox);
    for (int i = 1; i <= maxLength + 13; i++){
        if (i == maxLength + 1 || i == maxLength + 8)
            wprintw(win, "%s", downHorizontalBox);
        else
            wprintw(win, "%s", horizontalBox);
    }
    wprintw(win, "%s", downLeftBox);
    line++;

    wmove(win, y + 1, x);
    wprintw(win, "%sName", verticalBox);
    wmove(win, y + 1, x + maxLength + 1);
    wprintw(win, "%sPoints", verticalBox);
    wmove(win, y + 1, x + maxLength + 8);
    wprintw(win, "%sScore%s", verticalBox, verticalBox);
    line++;

    wmove(win, y + line, x);
    wprintw(win, "%s", verticalRightBox);
    for (int i = 1; i <= maxLength + 13; i++)
        if (i == maxLength + 1 || i == maxLength + 8)
            wprintw(win, "%s", verticalHorizontalBox);
        else
            wprintw(win, "%s", horizontalBox);
    wprintw(win, "%s", verticalLeftBox);
    line++;

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        if(game->teams[i] != NULL) {
            int playersNumber = 0; 
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++)
                if (game->teams[i]->players[j] != NULL) {
                    wmove(win, y + line, x);
                    wprintw(win, "%s%s", verticalBox,
                                         game->teams[i]->players[j]->name);
                    wmove(win, y + line, x + maxLength + 1);
                    wprintw(win, "%s", verticalBox);
                    wmove(win, y + line, x + maxLength + 8);
                    wprintw(win, "%s", verticalBox);
                    wmove(win, y + line, x + maxLength + 14);
                    wprintw(win, "%s", verticalBox);
                    line++;
                    wmove(win, y + line, x);
                    wprintw(win, "%s", verticalRightBox);
                    for (int k = 1; k <= maxLength + 14; k++) {
                        if (k <= maxLength)
                            wprintw(win, "%s", horizontalBox);
                        if (k == maxLength + 1)
                            wprintw(win, "%s", verticalLeftBox);
                        if (k == maxLength + 8 || k == maxLength + 14) {
                            wmove(win, y + line, x + k);
                            wprintw(win, "%s", verticalBox);
                        }
                    }
                    line++;
                    playersNumber++; 
                }
            --line;
            wmove(win, y + line, x + maxLength + 1);
            for (int k = maxLength + 1; k < maxLength + 14; k++) {
                if (k == maxLength + 1 || k == maxLength + 8)
                    wprintw(win, "%s", verticalHorizontalBox);
                else
                    wprintw(win, "%s", horizontalBox);
            }
            wprintw(win, "%s", verticalLeftBox);
            if (playersNumber > 0) {
                wmove(win, y + line - playersNumber, x + maxLength + 2);
                wprintw(win, "%*d", 6, round_computePoints(game->teams[i],
                                       round));
                wmove(win, y + line - playersNumber, x + maxLength + 9);
                wprintw(win, "%*d", 5, game->teams[i]->score);
            }
            line++;
        }

    wmove(win, y + line - 1, x);
    wprintw(win, "%s", upRightBox); 
    for (int i = 1; i <= maxLength + 13; i++) {
        if (i == maxLength + 1 || i == maxLength + 8)
            wprintw(win, "%s", upHorizontalBox);
        else
            wprintw(win, "%s", horizontalBox);
    }
    wprintw(win, "%s\n", upLeftBox);
    wrefresh(win);

    return NO_ERROR;
}

void createEmptyTeams(struct Game *game)
{
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            struct Team *team = team_createTeam();
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

    struct Player* backup = game->players[1];
    game->players[1]      = game->players[ch-'0'];
    game->players[ch-'0'] = backup;

    for (int i = 0; i < 2; i++) {
        struct Team *team = team_createTeam();
        team_addPlayer(team, game->players[2 * i]);
        team_addPlayer(team, game->players[2 * i + 1]);
        game_addTeam(team, game);
    }

    return NO_ERROR;
}

int displayCardsAndPickCard(struct Game *game, const int playerId)
{
    int colorPair;
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

    WINDOW *trumpAndTurnWindow = newwin(2, 29, 0, 0);
#ifdef BORDERS
    box(trumpAndTurnWindow, 0, 0);
#endif

    wprintw(trumpAndTurnWindow, "%s's turn:\n", player->name);

    if (game->round->trump != SuitEnd) {
        switch (game->round->trump) {
            case DIAMONDS:
                colorPair = 3;
                break;
            case CLUBS:
                colorPair = 4;
                break;
            case HEARTS:
                colorPair = 1;
                break;
            case SPADES:
                colorPair = 2;
                break;
            default:
                return ILLEGAL_VALUE;
        }
        wprintw(trumpAndTurnWindow, "Trump: ");
        wattron(trumpAndTurnWindow, COLOR_PAIR(colorPair));
        wprintw(trumpAndTurnWindow, "%s\n", suit);
        wattroff(trumpAndTurnWindow, COLOR_PAIR(colorPair));
    }
    else
        wprintw(trumpAndTurnWindow, "Trump was not set.\n");

    wrefresh(trumpAndTurnWindow);
    delwin(trumpAndTurnWindow);
    refresh();

    WINDOW *cardsOnTableWindow = newwin(10, 30, 2, 0);
#ifdef BORDERS
    box(cardsOnTableWindow, 0, 0);
#endif
    wprintw(cardsOnTableWindow, "Table cards: \n");
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->cards[i] != NULL)
            printCard(hand->cards[i], 7, cardsOnTableWindow);
    wrefresh(cardsOnTableWindow);

    refresh();

    WINDOW *cardsInHandWindow = newwin(10, 79, 12, 0); //MAGIC NUMBERS
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
            case 'a':
            case KEY_LEFT:
                selected = game_findPreviousAllowedCard(player, game, hand,
                                                        selected);
                break;
            case 'd':
            case KEY_RIGHT:
                selected = game_findNextAllowedCard(player, game, hand,
                                                    selected);
                break;
            case 'q':
                endwin();
                exit(0);
        }
        wclear(cardsInHandWindow);
        printPlayerCards(game, player, selected, cardsInHandWindow);
        wrefresh(cardsInHandWindow);
    }

    move(20, 0);
    if (handId == 0 && playerId == 0)
        game->round->trump=player->hand[selected]->suit;
    round_putCard(player, selected, handId, game->round);

    if (playerId == game->numberPlayers - 1) {
        wclear(cardsOnTableWindow);
        wprintw(cardsOnTableWindow, "Table cards: \n");
        for (int i = 0; i < MAX_GAME_PLAYERS; i++)
            if (hand->cards[i] != NULL)
                printCard(cardsOnTableWindow, hand->cards[i], 7);
        wrefresh(cardsOnTableWindow);

        wclear(cardsInHandWindow);
        printPlayerCards(game, player, selected, cardsInHandWindow);
        wrefresh(cardsInHandWindow);
        sleep(SLEEP_TIME);
    }

    delwin(cardsOnTableWindow);
    delwin(cardsInHandWindow);


    return NO_ERROR;
}

int printBids(int selected, struct Round *round, WINDOW *win)
{
    if (selected > 6 || selected < 0)
        return ILLEGAL_VALUE;
    if (round == NULL)
        return ROUND_NULL;

    for (int i = 0; i <= 6; i++)
        if (i == selected) {
            wattron(win, COLOR_PAIR(3));
            wprintw(win, "%d ", i);
            wattroff(win, COLOR_PAIR(3));
        }
        else if (i > round_getMaximumBid(round) || i == 0)
                wprintw(win, "%d ", i);
             else {
                 wattron(win, COLOR_PAIR(1));
                 wprintw(win, "%d ", i);
                 wattroff(win, COLOR_PAIR(1));
             }

    return NO_ERROR;
}

int getBid(const struct Game *game, const int playerId)
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

    displayBids(game, playerId);

    getyx(stdscr, y, x);
    refresh();

    WINDOW *bidsWindow = newwin(1, 30, y, 0);
#ifdef BORSERS
    box(bidsWindow, 0, 0);
#endif
    keypad(bidsWindow, TRUE);

    int ch, selected = 0;
    wprintw(bidsWindow, "Choose a bid: ");
    printBids(selected, game->round, bidsWindow);
    while((ch = wgetch(bidsWindow)) != '\n') {
        switch (ch) {
            case 'a':
            case KEY_LEFT:
                selected = round_findPreviousAllowedBid(game->round, selected);
                break;
            case 'd':
            case KEY_RIGHT:
                selected = round_findNextAllowedBid(game->round, selected);
                break;
            case 'q':
                endwin();
                exit(0);
        }
        wclear(bidsWindow);
        wprintw(bidsWindow, "Choose a bid: ");
        printBids(selected, game->round, bidsWindow);
        wrefresh(bidsWindow);
    }

    delwin(bidsWindow);
    
    round_placeBid(game->round->players[playerId], selected, game->round);

    return NO_ERROR;
}

int processingScore(const char *score)
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
    if (strcmp(score, "q") == 0) {
        endwin();
        exit(0);
    }

    return -1;
#endif
}

int getScoreLimit()
{
    char score[3];
    int length;

    do{
        printw("Insert the score limit (11-15-21): ");
        scanw("%s", score);
        length = strlen(score);
    } while(length == 0);

    while (processingScore(score) == -1) {
        printw("Insert the correct score limit (11-15-21): ");
        scanw("%s", score);
    }
    printw("\n");

    return processingScore(score);
}
/**
 * @brief Computes the size of line in the score dialog. The 
 *        size of a line consists in the sum of the lengths 
 *        of the name plus the number of commas and white spaces
 *        between them. This function is a helper for 
 *        getBiggestScoreDialogLineSize and printRoundTerminationMessage.
 *
 * @param currentTeam The desired team that size we want to   
 *        compue.
 *
 * @return int The size of a team. 
 */
int getScoreDialogLineSize(const struct Team *currentTeam)
{ 
   int lineSize = 0;
   int i;
   for(i = 0; i < MAX_TEAM_PLAYERS; i++) {
       if(currentTeam->players[i] != NULL)
           lineSize += strlen(currentTeam->players[i]->name);
   }
   lineSize += i + i - 1;
   return lineSize;
}

/**
 * @brief Get the biggest line's size. This function is a helper for
 *        printRoundTerminationMessage.
 *
 * @param currentGame The game in which these teams reside.
 *
 * @return int The size of the biggest line.
 */
 int getBiggestScoreDialogLineSize(const struct Game *currentGame)
 {
    int biggestLineSize = 0;
    for(int i = 0; i < MAX_GAME_TEAMS; i++) {
        if(currentGame->teams[i] != NULL) { 
            int currentLine = getScoreDialogLineSize(currentGame->teams[i]);
            if(currentLine > biggestLineSize) {
               biggestLineSize = currentLine;
            }
        }
    }

    return biggestLineSize;
}

int printRoundTerminationMessage(const struct Game *currentGame, 
                                 const int *oldScore)
{
    if(currentGame->round == NULL || currentGame->round->players == NULL)
        return ROUND_NULL;

    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);

    int scoreLineSize = getBiggestScoreDialogLineSize(currentGame) + 
                                           ROUND_DIALOG_SCORE_SIZE;

    printw("  _____                       _        _     _ \n"     
             " / ____|                     | |      | |   | |     \n"
            " | (___   ___ ___  _ __ ___  | |_ __ _| |__ | | ___ \n"
             "  \\___ \\ / __/ _ \\| '__/ _ \\ | __/ _` | '_ \\| |/ _ \\\n"
             "  ____) | (_| (_) | | |  __/ | || (_| | |_) | |  __/\n"
            " |_____/ \\___\\___/|_|  \\___|  \\__\\__,_|_.__/|_|\\___|\n\n\n");
     for(int i = 0; i < MAX_GAME_TEAMS; i++) {
        if(currentGame->teams[i] != NULL) {
            for(int j = 0; j < 2; j++) {
                if(currentGame->teams[i]->players[j] != NULL) {
                   printw("%s%s ", currentGame->teams[i]->players[j]->name, 
                            ((currentGame->teams[i]->players[j + 1] == NULL) 
                             ? "" : ","));
                }
            }
            int currentTeamSize = getScoreDialogLineSize(currentGame->teams[i]);
            int score = currentGame->teams[i]->score - oldScore[i];         
            int colorPair = (score > 0) ? 2 : 1;

            if(score != 0) { 
                attron(COLOR_PAIR(colorPair));
                printw("%*d\n", scoreLineSize - currentTeamSize,
                        currentGame->teams[i]->score);
                attroff(COLOR_PAIR(colorPair));
            } else {
                printw("%*d\n", scoreLineSize - currentTeamSize,
                        currentGame->teams[i]->score);
            }
        }
    }

    return NO_ERROR;
}

int displayBids(const struct Game *game, const int currentPlayer)
{
    if (game == NULL)
        return GAME_NULL;
    if (game->round == NULL)
        return ROUND_NULL;
    if (currentPlayer > game->numberPlayers || currentPlayer < 0)
        return ILLEGAL_VALUE;

    for (int i = 0; i < game->numberPlayers; i++)
        if (game->round->players[i] != NULL)
            if (i < currentPlayer)
                printw("%s bid %d\n", game->round->players[i]->name, 
                                      game->round->bids[i]);
            else
                printw("%s bid -\n", game->round->players[i]->name);

    return NO_ERROR;
}

