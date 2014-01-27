#include <round.h>
#include <cutter.h>
#include <stdlib.h>
#include <stdio.h>

static struct Hand *hand;
static struct Player *player1;
static struct Player *player2;
static struct Player *player3;
static struct Player *player4;

void cut_setup()
{
    hand = round_createHand();

    player1 = team_createPlayer("A", 0, 0);
    player2 = team_createPlayer("A", 0, 0);
    player3 = team_createPlayer("A", 0, 0);
    player4 = team_createPlayer("A", 0, 0);
}

void cut_teardown()
{
    round_deleteHand(&hand);

    team_deletePlayer(&player1);
    team_deletePlayer(&player2);
    team_deletePlayer(&player3);
    team_deletePlayer(&player4);
}

void test_round_createRound()
{
    struct Round *round = round_createRound(DIAMONDS);

    cut_assert_equal_int(round->trump, DIAMONDS);

    for (int i = 0; i < MAX_HANDS; i++)
        cut_assert_equal_pointer(round->hands[i], NULL);

    round_deleteRound(&round);
}

void test_round_createHand()
{
    struct Hand *hand = round_createHand();

    for(int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_pointer(hand->cards[i], NULL);
        cut_assert_equal_pointer(hand->players[i], NULL);
        cut_assert_equal_int(hand->bids[i], 0);
    }
}

void test_round_deleteRound()
{
    struct Round *round = round_createRound(DIAMONDS);
    cut_assert_equal_int(NO_ERROR, round_deleteRound(&round));
    cut_assert_equal_pointer(round, NULL);
    cut_assert_equal_int(POINTER_NULL, round_deleteRound(NULL));
    cut_assert_equal_int(ROUND_NULL, round_deleteRound(&round));
}

void test_round_deleteHand()
{
    struct Hand *hand = round_createHand();
    cut_assert_equal_int(NO_ERROR, round_deleteHand(&hand));
    cut_assert_equal_pointer(hand, NULL);
    cut_assert_equal_int(POINTER_NULL, round_deleteHand(NULL));
    cut_assert_equal_int(HAND_NULL, round_deleteHand(&hand));
}


void test_round_addPlayer()
{
    cut_assert_not_equal_int(0, round_addPlayer(NULL, hand));
    cut_assert_not_equal_int(0, round_addPlayer(player1, NULL));
    cut_assert_not_equal_int(0, round_addPlayer(NULL, NULL));

    cut_assert_equal_int(0, round_addPlayer(player1, hand));
    cut_assert_equal_pointer(hand->players[1], NULL);

    cut_assert_equal_int(0, round_addPlayer(player2, hand));
    cut_assert_equal_pointer(hand->players[2], NULL);

    cut_assert_equal_int(0, round_addPlayer(player3, hand));
    cut_assert_equal_pointer(hand->players[3], NULL);

    cut_assert_equal_int(0, round_addPlayer(player4, hand));
    cut_assert_equal_pointer(hand->players[4], NULL);

    cut_assert_equal_pointer(hand->players[0], player1);
    cut_assert_equal_pointer(hand->players[1], player2);
    cut_assert_equal_pointer(hand->players[2], player3);
    cut_assert_equal_pointer(hand->players[3], player4);
}

void printHand()
{
    fprintf(stderr, "%p %p\n", player1, hand->players[0]);
    fprintf(stderr, "%p %p\n", player2, hand->players[1]);
    fprintf(stderr, "%p %p\n", player3, hand->players[2]);
    fprintf(stderr, "%p %p\n", player4, hand->players[3]);
}

void printBids()
{
    fprintf(stderr, "%d\n", hand->bids[0]);
    fprintf(stderr, "%d\n", hand->bids[1]);
    fprintf(stderr, "%d\n", hand->bids[2]);
    fprintf(stderr, "%d\n", hand->bids[3]);
}

void test_round_placeBid()
{
    round_addPlayer(player1, hand);
    round_addPlayer(player2, hand);
    round_addPlayer(player3, hand);
    round_addPlayer(player4, hand);

    cut_assert_not_equal_int(0, round_placeBid(NULL, 2, hand));
    cut_assert_not_equal_int(0, round_placeBid(player1, -1, hand));
    cut_assert_not_equal_int(0, round_placeBid(player1, 2, NULL));

    cut_assert_equal_int(0, round_placeBid(player1, 0, hand));
    cut_assert_equal_int(hand->bids[0], 0);

    cut_assert_equal_int(0, round_placeBid(player2, 2, hand));
    cut_assert_equal_int(hand->bids[1], 2);

    cut_assert_equal_int(0, round_placeBid(player3, 3, hand));
    cut_assert_equal_int(hand->bids[2], 3);

    cut_assert_equal_int(0, round_placeBid(player4, 4, hand));
    cut_assert_equal_int(hand->bids[3], 4);
}

void test_round_getBidWinner()
{
    round_addPlayer(player1, hand);
    round_addPlayer(player2, hand);
    round_addPlayer(player3, hand);
    round_addPlayer(player4, hand);

    round_placeBid(player1, 0, hand);
    round_placeBid(player2, 3, hand);
    round_placeBid(player3, 2, hand);
    round_placeBid(player4, 4, hand);

    cut_assert_equal_pointer(NULL, round_getBidWinner(NULL));

    cut_assert_equal_pointer(player4, round_getBidWinner(hand));
}


