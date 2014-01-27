#include <round.h>
#include <cutter.h>
#include "../src/errors.h"

#include <stdio.h>

static struct Hand *hand;
static struct Player *players[MAX_GAME_PLAYERS];

void cut_setup()
{
    hand = round_createHand();

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        players[i] = team_createPlayer("A", 0, 0);
}

void cut_teardown()
{
    round_deleteHand(&hand);

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        team_deletePlayer(&players[i]);
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
    for (int i = 0; i < MAX_GAME_PLAYERS; i++){
        fprintf(stderr, "%p\n",players[i]);
    } 
    cut_assert_equal_int(PLAYER_NULL, round_addPlayer(NULL, hand));
    cut_assert_equal_int(HAND_NULL, round_addPlayer(players[0], NULL));
    cut_assert_not_equal_int(0, round_addPlayer(NULL, NULL));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));

        int found = 0;
        for(int j = 0; j < MAX_GAME_PLAYERS; i++)
            if (players[i] == hand->players[j])
                found++;
        cut_assert_equal_int(found, 0);
    }

    cut_assert_equal_int(FULL, round_addPlayer(players[0], hand));
}
void test_round_placeBid()
{
    cut_assert_equal_int(PLAYER_NULL, round_placeBid(NULL, 2, hand));
    cut_assert_equal_int(ILLEGAL_VALUE, round_placeBid(players[0], -1, hand));
    cut_assert_not_equal_int(NO_ERROR, round_placeBid(players[0], 2, NULL));

 //   cut_assert_equal_int(NO_ERROR, round_addPlayer(players[0], hand));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++){
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
  //      cut_assert_equal_int(NO_ERROR, round_placeBid(players[i], i, hand));

     //   for(int j = 0; j < MAX_GAME_PLAYERS; i++)
    //        if (players[i] == hand->players[j])
   //             cut_assert_equal_int(hand->bids[j], i);
    }

}

void test_round_getBidWinner()
{
/*    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, round_placeBid(players[i], i, hand));
    }

    cut_assert_equal_pointer(NULL, round_getBidWinner(NULL));

    cut_assert_equal_pointer(players[MAX_GAME_PLAYERS - 1],
                             round_getBidWinner(hand));
*/
}


