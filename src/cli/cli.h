/**
 * @file cli.h
 * @brief Contains declaration of the functions used for building the user
 *        interface.
 */
 
#include <cruceGame.h>
#include <ncurses.h>
#include <network.h>

#define GAME_COMMAND_FLAG '#'

/**
 * @brief Prints the help manual of cruce game to the screen.
 *
 * @return void.
 */
void cruceGameHelp();

/**
 * @brief Function to display a greeting message at the beginning of a new
 *        game.
 * 
 * @param win Pointer to the window in which the output will be displayed.
 *
 * @return void.
 */
void welcomeMessage(WINDOW *win);

/**
 * @brief Function to display the ending message and the members of
 *        the winning team.
 *
 * @param win  Pointer to the window in which the output will be displayed.
 * @param team Pointer to the winning team.
 *
 * @return void
 */
 void gameEndingMessage(WINDOW *win, const struct Team *team);

/**
 * @brief Function to print a Card.
 *
 * @param win  Pointer to the window in which the output will be displayed.
 * @param card Pointer to the card to be printed.
 * @param frameColor The color with which is colored the frame of card.
 * @param win The window in which to display the card.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int printCard(WINDOW *win, const struct Card *card, const int frameColor);

/**
 * @brief Function to print all cards in a player's hand.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param player The player whose cards to be printed.
 * @param game The game which contains the player.
 * @param selected The position of the selected card. If there is no card at
 *                 that position (there is a NULL card or it is out of range)
 *                 no card will be selected.
 * @param win The window in which to display the cards.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int printPlayerCards(WINDOW *win, const struct Game *game, 
                     struct Player *player, const int selected);

/**
 * @brief Function to get the number of players from user.
 *
 * @param win Pointer to the window in which the data will be displayed.
 *
 * @return The number of players.
 */
int getNoOfPlayers(WINDOW *win);

/**
 * @brief Function to get user data and create a new Player.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param i The player number.
 *
 * @return Pointer to the new Player.
 */
struct Player *newPlayer(WINDOW *win, const int i);

/**
 * @brief Function to print the score table.
 *
 * @param win Pointer to the window in which to display the score.
 * @param game Pointer to the game of where to be printed the score.
 * @param round Pointer to the round of where to be printed the points.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int printScore(WINDOW *win, const struct Game *game, const struct Round *round);

/**
 * @brief Function to form the teams.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param game Pointer to the game where form it the teams.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int formTeams(WINDOW *win, struct Game* game);

/**
 * @brief Function displays the trump, the cards on table, the player's cards
 *        and puts the player to pick a card.
 *
 * @param game Pointer to the game where to be the player and the table.
 * @param playerId The player's id to whom displays the cards.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int displayCardsAndPickCard(struct Game *game, const int playerId);

/*
 * @brief Function to display player's cards and ask for a bid.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param game Pointer to the Game.
 * @param playerId Id of the bidding player.
 *
 * @return \ref NO_ERROR or 0 un success, other value on failure.
 */
int getBid(WINDOW *win, const struct Game* game, const int playerId);

/**
 * @brief Function to get the score limit from user.
 *
 * @param win Pointer to the window in which the data will be displayed.
 *
 * @return The score limit (11-15-21).
 */
int getScoreLimit(WINDOW *win);

/**
 * @brief Prints the score differences between rounds.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param currentGame The current game.
 * @param oldScore Old score for every player before the terminated round.
 *
 * @return int The size of the biggest name of a player in a round.
 */
int printRoundTerminationMessage(WINDOW *win, const struct Game *currentGame,
                                 const int *oldScore);

/**
 * @brief Displays how much bid each player.
 *
 * @param win Pointer to the window in which the data will be displayed.
 * @param game The game in which are the players.
 * @param currentPlayer The player which have to bid.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int displayBids(WINDOW *win, const struct Game *game, const int currentPlayer);

/**
 * @brief Displays the bids for to pick a bid.
 *
 * @param win The window in which are display the bids.
 * @param selected The selected bid.
 * @param round The round in which are the bids.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
int printBids(WINDOW *win, int selected, struct Round *round);

/**
 * @brief Displays bid summary.
 *
 * @param win Window where to display the bid summary.
 * @param game The game for which the bid summary is generated.
 *
 * @return void.
 */
void bidSummary(WINDOW *win, struct Game *game);

/**
 * @brief Function to get current player's name.
 *
 * @param win Pointer to the window from which the input will be obtained.
 * @return Pointer to the string which contains the name of the player.
 *         WARNING: THIS MUST BE FREED.
 */
char *getPlayerName(WINDOW *win);

/**
 * @brief Function to initialize the windows used for lobby (readWin and
 *        writeWin) defined in cli.c.
 *
 * @return void.
 */
void initWindows();

/**
 * @brief Handler for IRC PRIVMSG command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onPRIVMSG_handler(struct Privmsg *privmsg);

/**
 * @brief Handler for IRC JOIN command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onJOIN_handler(struct Message *message);

/**
 * @brief Handler for IRC QUIT command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onQUIT_handler(struct Message *message);

WINDOW *getReadWin(); //TEMPORARY
WINDOW *getPrintWin();
