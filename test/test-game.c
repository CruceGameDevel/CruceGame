#include <game.h>
#include <errors.h>
#include <constants.h>
#include <team.h>

#include<string.h>

#include <cutter.h>
#include <stdio.h>

void test_game_createGame()
{
    struct Game *game = game_createGame(11);

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_pointer(NULL, game->players[i]);

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        cut_assert_equal_pointer(NULL, game->teams[i]);

#ifndef DEBUG
    cut_assert_equal_pointer(NULL, game_createGame(0));
#endif
    cut_assert_equal_pointer(NULL, game->deck);
    cut_assert_equal_pointer(NULL, game->round);
    cut_assert_equal_int(0, game->numberPlayers);
    cut_assert_equal_int(11, game->pointsNumber);

    game_deleteGame(&game);
}

void test_game_deleteGame()
{
    struct Game *game = game_createGame(11);

    cut_assert_equal_int(NO_ERROR, game_deleteGame(&game));
    cut_assert_equal_pointer(NULL, game);
    cut_assert_equal_int(POINTER_NULL, game_deleteGame(NULL));
    cut_assert_equal_int(GAME_NULL, game_deleteGame(&game));
}

void test_game_addPlayer()
{
    struct Game *game = game_createGame(11);
    struct Player *player[MAX_GAME_PLAYERS];

    char *names[] = {"A", "B", "C", "D"};
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        player[i] = team_createPlayer(names[i], i);
        cut_assert_equal_int(NO_ERROR, game_addPlayer(player[i], game));
        cut_assert_equal_int(i+1, game->numberPlayers);
        cut_assert_equal_int(DUPLICATE, game_addPlayer(player[i], game));
        int playerAdded = -1;
        for (int j = 0; j < MAX_GAME_PLAYERS; j++)
            if (game->players[j] == player[i])
                playerAdded = 0;
        cut_assert_equal_int(0, playerAdded);
    }

    struct Player *player1 = team_createPlayer("E", 0);
    cut_assert_equal_int(GAME_NULL, game_addPlayer(player1, NULL));
    cut_assert_equal_int(PLAYER_NULL, game_addPlayer(NULL, game));
    cut_assert_operator_int(0, >, game_addPlayer(NULL, NULL));
    cut_assert_equal_int(FULL, game_addPlayer(player1, game));
    game_removePlayer(player[0], game);
    team_deletePlayer(&player1);
    player1 = team_createPlayer("C", 0);
    cut_assert_equal_int(DUPLICATE_NAME, game_addPlayer(player1, game));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&player[i]);

    team_deletePlayer(&player1);
    game_deleteGame(&game);
}

void test_game_removePlayer()
{
    struct Game *game = game_createGame(11);
    struct Player *player[MAX_GAME_PLAYERS];

    char *names[] = {"A", "B", "C", "D"};
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        player[i] = team_createPlayer(names[i], i);
        game_addPlayer(player[i], game);
    }

    cut_assert_equal_int(GAME_NULL, game_removePlayer(player[0], NULL));
    cut_assert_equal_int(PLAYER_NULL, game_removePlayer(NULL, game));
    cut_assert_operator_int(0, >, game_removePlayer(NULL, NULL));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, game_removePlayer(player[i], game));
        cut_assert_equal_int(NOT_FOUND, game_removePlayer(player[i], game));
        int playerRemoved = -1;
        for (int j = 0; j < MAX_GAME_PLAYERS; j++)
            if (game->players[j] == player[i])
                playerRemoved = 0;
        cut_assert_equal_int(-1, playerRemoved);
        team_deletePlayer(&player[i]);
    }

    game_deleteGame(&game);
}

void test_addTeam()
{
    struct Game *game = game_createGame(11);
    struct Team *team[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        team[i] = team_createTeam();
        cut_assert_equal_int(NO_ERROR, game_addTeam(team[i], game));
        cut_assert_equal_int(DUPLICATE, game_addTeam(team[i], game));
        int teamAdded = -1;
        for (int j = 0; j < MAX_GAME_TEAMS; j++)
            if (game->teams[j] == team[i])
                teamAdded = 0;
        cut_assert_equal_int(0, teamAdded);
    }

    struct Team *team1 = team_createTeam();
    cut_assert_equal_int(GAME_NULL, game_addTeam(team1, NULL));
    cut_assert_equal_int(TEAM_NULL, game_addTeam(NULL, game));
    cut_assert_operator_int(0, >, game_addTeam(NULL, NULL));
    cut_assert_equal_int(FULL, game_addTeam(team1, game));

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        team_deleteTeam(&team[i]);

    team_deleteTeam(&team1);
    game_deleteGame(&game);
}

void test_game_removeTeam()
{
    struct Game *game = game_createGame(11);
    struct Team *team[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        team[i] = team_createTeam();
        game_addTeam(team[i], game);
    }

    cut_assert_equal_int(GAME_NULL, game_removeTeam(team[0], NULL));
    cut_assert_equal_int(TEAM_NULL, game_removeTeam(NULL, game));
    cut_assert_operator_int(0, >, game_removeTeam(NULL, NULL));

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        cut_assert_equal_int(NO_ERROR, game_removeTeam(team[i], game));
        cut_assert_equal_int(NOT_FOUND, game_removeTeam(team[i], game));
        int teamRemoved = -1;
        for (int j = 0; j < MAX_GAME_TEAMS; j++)
            if (game->teams[j] == team[i])
                teamRemoved = 0;
        cut_assert_equal_int(-1, teamRemoved);
        team_deleteTeam(&team[i]);
    }

    game_deleteGame(&game);
}

void test_game_winningTeam()
{
    cut_assert_equal_pointer(NULL, game_winningTeam(NULL));

    struct Game *game = game_createGame(11);
    struct Team *teams[MAX_GAME_TEAMS];

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        teams[i] = team_createTeam();
        game_addTeam(teams[i], game);
    }

    cut_assert_equal_pointer(NULL, game_winningTeam(game));

    teams[0]->score = 11;
    cut_assert_equal_pointer(teams[0], game_winningTeam(game));

    teams[1]->score = 11;
    cut_assert_equal_pointer(NULL, game_winningTeam(game));
    cut_assert_equal_int(21, game->pointsNumber);

    game->pointsNumber = 11;

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        team_deleteTeam(&teams[i]);
    }
    game_deleteGame(&game);
}

void test_game_checkCard()
{
    struct Player *player [3];
    struct Game *game = game_createGame(11);
    struct Hand *hand = round_createHand();
    struct Round *round = round_createRound();
    struct Card *card[10];
    round->hands[0] = hand;
    game->round = round;

    char *names[] = {"A", "B", "C"};
    for (int i = 0; i < 3; i++)
        player[i] = team_createPlayer(names[i], i);

    cut_assert_equal_int(PLAYER_NULL, game_checkCard(NULL, game, hand, 0));
    cut_assert_equal_int(GAME_NULL, game_checkCard(player[2], NULL, hand, 0));
    cut_assert_equal_int(HAND_NULL, game_checkCard(player[2], game, NULL, 0));
    cut_assert_equal_int(GAME_EMPTY, game_checkCard(player[2], game, hand, 0));
    game_addPlayer(player[0], game);
    game_addPlayer(player[1], game); 
    game_addPlayer(player[2], game); 
    cut_assert_equal_int(ILLEGAL_VALUE,
                         game_checkCard(player[2], game, hand, -1));
    cut_assert_equal_int(ILLEGAL_VALUE,
                         game_checkCard(player[2], game, hand, 10));
    cut_assert_equal_int(CARD_NULL, game_checkCard(player[2], game, hand, 0));

    card[0] = deck_createCard(DIAMONDS, VALUES[1]);
    card[1] = deck_createCard(DIAMONDS, VALUES[5]);
    card[2] = deck_createCard(CLUBS, VALUES[2]);
    card[3] = deck_createCard(CLUBS, VALUES[3]);
    card[4] = deck_createCard(SPADES, VALUES[0]);
    card[5] = deck_createCard(SPADES, VALUES[1]);
    card[6] = deck_createCard(HEARTS, VALUES[2]);
    card[7] = deck_createCard(HEARTS, VALUES[4]);
    card[8] = deck_createCard(DIAMONDS, VALUES[0]);
    card[9] = deck_createCard(DIAMONDS, VALUES[3]);

    for (int i = 0; i < 8; i++) {
        team_addCard(player[0], card[i]);
        cut_assert_equal_int(1, game_checkCard(player[0], game, hand, i));
    }

    team_addCard(player[1], card[8]);
    team_addCard(player[2], card[9]);
    round_addPlayerHand(player[1], hand);
    round_addPlayerHand(player[2], hand);
    round_putCard(player[1], 0, 0, round);
    round_putCard(player[2], 0, 0, round);

    //the first card is trump
    game->round->trump = DIAMONDS;
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 0));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 3));

    card[9]->value = VALUES[4];
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 0));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 5));
    
    card[9]->value = VALUES[3];
    //the first card not is trump
    game->round->trump = CLUBS;
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 0)); 
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 2));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 4));

    card[9]->value = VALUES[4];
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 0));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1)); 
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 2));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 4));

    //the first card is trump and the player doesn't has trump
    deck_deleteCard(&player[0]->hand[2]);
    deck_deleteCard(&player[0]->hand[3]);
    card[8]->suit = CLUBS;
    card[9]->suit = CLUBS;
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 0));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 4));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 6));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 7));

    //the first card not is trump and the player doesn't has same suit as
    //first card, but he has trump
    game->round->trump = DIAMONDS; 
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 0));
    cut_assert_equal_int(1, game_checkCard(player[0], game, hand, 1));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 4));
    cut_assert_equal_int(0, game_checkCard(player[0], game, hand, 7));

    //the player doesn't has same suit as first card and no trump
    deck_deleteCard(&player[0]->hand[0]);
    deck_deleteCard(&player[0]->hand[1]);
    for (int i = 4; i < 8; i++) {
        cut_assert_equal_int(1, game_checkCard(player[0], game, hand, i));
        deck_deleteCard(&player[0]->hand[i]);
    }

    team_deletePlayer(&player[0]);
    team_deletePlayer(&player[1]);
    team_deletePlayer(&player[2]);
    round_deleteRound(&round);
    round_deleteHand(&hand);
    game_deleteGame(&game);
}   

void test_game_findNextAllowedCard()
{
    struct Game *game = game_createGame(11);
    struct Hand *hand = round_createHand();
    struct Player *player = team_createPlayer("A", 0);
    struct Round *round = round_createRound();

    round->hands[0] = hand;
    game->round = round;
    game_addPlayer(player, game);
    round_addPlayerHand(player, hand);
    round_addPlayer(player, round);

    player->hand[0] = deck_createCard(DIAMONDS, VALUES[1]);
    player->hand[1] = deck_createCard(HEARTS, VALUES[2]);
    player->hand[2] = deck_createCard(CLUBS, VALUES[2]);
    player->hand[3] = deck_createCard(SPADES, VALUES[0]);
    player->hand[4] = deck_createCard(DIAMONDS, VALUES[5]);
    player->hand[5] = deck_createCard(SPADES, VALUES[1]);
    player->hand[6] = deck_createCard(HEARTS, VALUES[4]);
    player->hand[7] = deck_createCard(CLUBS, VALUES[3]);

    round->trump = HEARTS;

    for(int i = 0; i < 8; i++) {
        cut_assert_equal_int((i + 1) % 8, game_findNextAllowedCard(player, game,
                                                              hand, i));
    }

    for (int i = 0 ; i < 8; i++)
        deck_deleteCard(&player->hand[i]);

    team_deletePlayer(&player);
    round_deleteHand(&hand);
    round_deleteRound(&round);
    game_deleteGame(&game);
}

void test_game_findPreviousAllowedCard()
{
    struct Game *game = game_createGame(11);
    struct Hand *hand = round_createHand();
    struct Player *player = team_createPlayer("A", 0);
    struct Round *round = round_createRound();

    round->hands[0] = hand;
    game->round = round;
    game_addPlayer(player, game);
    round_addPlayerHand(player, hand);
    round_addPlayer(player, round);

    player->hand[0] = deck_createCard(DIAMONDS, VALUES[1]);
    player->hand[1] = deck_createCard(HEARTS, VALUES[2]);
    player->hand[2] = deck_createCard(CLUBS, VALUES[2]);
    player->hand[3] = deck_createCard(SPADES, VALUES[0]);
    player->hand[4] = deck_createCard(DIAMONDS, VALUES[5]);
    player->hand[5] = deck_createCard(SPADES, VALUES[1]);
    player->hand[6] = deck_createCard(HEARTS, VALUES[4]);
    player->hand[7] = deck_createCard(CLUBS, VALUES[3]);

    round->trump = HEARTS;

    for(int i = 0; i < 8; i++) {
        int value = i - 1;
        if (value < 0)
            value += MAX_CARDS;
        cut_assert_equal_int(value, game_findPreviousAllowedCard(player, game,
                                                              hand, i));
    }

    for (int i = 0 ; i < 8; i++)
        deck_deleteCard(&player->hand[i]);

    team_deletePlayer(&player);
    round_deleteHand(&hand);
    round_deleteRound(&round);
    game_deleteGame(&game);
}

void test_game_arrangePlayersRound()
{
    struct Game *game = game_createGame(11);

    cut_assert_equal_int(GAME_NULL, game_arrangePlayersRound(NULL, 1));
    cut_assert_equal_int(ILLEGAL_VALUE, game_arrangePlayersRound(game, -1));
    cut_assert_equal_int(ILLEGAL_VALUE,
                         game_arrangePlayersRound(game, MAX_GAME_PLAYERS));

    char *names[] = {"A", "B", "C", "D"};
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        struct Player *player = team_createPlayer(names[i], 0);
        game_addPlayer(player, game);
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, game_arrangePlayersRound(game, i));
        for (int j = 0; j < MAX_GAME_PLAYERS; j++)
            cut_assert_equal_pointer(game->round->players[j],
                                    game->players[(i + j) % MAX_GAME_PLAYERS]);
        round_deleteRound(&game->round);
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&game->players[i]);
    game_deleteGame(&game);
}

void test_game_findTeam()
{
    struct Game *game = game_createGame(11);
    struct Player *players[MAX_GAME_PLAYERS];
    struct Team *teams[MAX_GAME_TEAMS];
    struct Player *player = team_createPlayer("E", 1);

    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        teams[i] = team_createTeam();
        game_addTeam(teams[i], game);
    }

    cut_assert_equal_pointer(NULL, game_findTeam(NULL, NULL));
    cut_assert_equal_pointer(NULL, game_findTeam(NULL, player));
    cut_assert_equal_pointer(NULL, game_findTeam(game, NULL));
    cut_assert_equal_pointer(NULL, game_findTeam(game, player));

    char *names[] = {"A", "B", "C", "D"};
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        players[i] = team_createPlayer(names[i], i);
        team_addPlayer(teams[i % 2], players[i]);
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_pointer(teams[i % 2], game_findTeam(game, players[i]));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&players[i]);

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        team_deleteTeam(&teams[i]);

    team_deletePlayer(&player);
    game_deleteGame(&game);
}

void updateScore_generateTest(int bids[4], int pointsNumber[4], 
                              int scores[4], struct Game *game) {
    memcpy(game->round->bids, bids, 4 * sizeof(int));
    memcpy(game->round->pointsNumber, pointsNumber, 4 * sizeof(int));
    struct Player *bidWinner = round_getBidWinner(game->round);
    game_updateScore(game, bidWinner);
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        if (game->teams[i] != NULL) {
            cut_assert_equal_int(game->teams[i]->score, scores[i]);
            for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
                if (game->teams[i]->players[j] != NULL) {
                    cut_assert_equal_int(game->teams[i]->players[j]->score,
                                         game->teams[i]->score);
                    game->teams[i]->players[j]->score = 0;
                }
            }
            game->teams[i]->score = 0;
        }
    }
}



void test_game_updateScore()
{
    struct Game *game = game_createGame(11);
    char *names[] = {"A", "B", "C", "D"};
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        struct Player *player = team_createPlayer(names[i], 0);
        game_addPlayer(player, game);
    }
    for (int i = 0; i < MAX_GAME_TEAMS; i++) {
        struct Team *team = team_createTeam();
        team_addPlayer(team, game->players[i]);
        game_addTeam(team, game);
    }
    game_arrangePlayersRound(game, 0);

    //test1
    int bids[4] = {0, 2, 3, 0};
    int pointsNumber[4] = {10, 66, 100, 53};
    int scores[4] = {0, 2, 3, 1};

    updateScore_generateTest(bids, pointsNumber, scores, game);

    //test2
    pointsNumber[2] = 33;
    scores[2] = -3;

    updateScore_generateTest(bids, pointsNumber, scores, game);

    //test3
    bids[1] = 5;
    bids[2] = 0;
    bids[3] = 0;

    scores[1] = -5;
    scores[2] = 1;

    updateScore_generateTest(bids, pointsNumber, scores, game);

    //test4
    pointsNumber[1] = 0;

    updateScore_generateTest(bids, pointsNumber, scores, game);

    //test5
    game->teams[2] = NULL;
    game->teams[3] = NULL;

    team_addPlayer(game->teams[0], game->players[2]);
    team_addPlayer(game->teams[1], game->players[3]);

    scores[0] = 1;
    scores[1] = -5;

    updateScore_generateTest(bids, pointsNumber, scores, game);

    //test6
    pointsNumber[1] = 66;
    pointsNumber[3] = 100;
    scores[1] = 5;

    updateScore_generateTest(bids, pointsNumber, scores, game);
}

