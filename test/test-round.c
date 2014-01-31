#include <round.h>
#include <errors.h>
#include <constants.h>
#include <team.h>
#include <deck.h>

#include <cutter.h>

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
    cut_assert_equal_int(PLAYER_NULL, round_addPlayer(NULL, hand));
    cut_assert_equal_int(HAND_NULL, round_addPlayer(players[0], NULL));
    cut_assert_not_equal_int(0, round_addPlayer(NULL, NULL));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));
        cut_assert_equal_int(DUPLICATE, round_addPlayer(players[i], hand));

        int found = 0;
        for(int j = 0; j < MAX_GAME_PLAYERS; j++) {
            if (players[i] == hand->players[j])
                found++;
        }
        cut_assert_equal_int(found, 1);
    }

}

void test_round_placeBid()
{
    cut_assert_equal_int(PLAYER_NULL, round_placeBid(NULL, 2, hand));
    cut_assert_equal_int(ILLEGAL_VALUE, round_placeBid(players[0], -1, hand));
    cut_assert_not_equal_int(NO_ERROR, round_placeBid(players[0], 2, NULL));


    for (int i = 0; i < MAX_GAME_PLAYERS; i++){
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, round_placeBid(players[i], i, hand));

        int found = 0;
        for(int j = 0; j < MAX_GAME_PLAYERS; j++)
            if (players[i] == hand->players[j] && hand->bids[j] == i)
                found++;

        cut_assert_equal_int(found, 1);
    }
}

void test_round_getBidWinner()
{
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, round_placeBid(players[i], i, hand));
    }

    cut_assert_equal_pointer(NULL, round_getBidWinner(NULL));

    cut_assert_equal_pointer(players[MAX_GAME_PLAYERS - 1],
                             round_getBidWinner(hand));
}

void test_round_removePlayer()
{
    cut_assert_equal_int(PLAYER_NULL, round_removePlayer(NULL, hand));
    cut_assert_equal_int(HAND_NULL, round_removePlayer(players[0], NULL));
    cut_assert_not_equal_int(NO_ERROR, round_removePlayer(NULL, NULL));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        cut_assert_equal_int(NO_ERROR, round_addPlayer(players[i], hand));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++){
        cut_assert_equal_int(NO_ERROR, round_removePlayer(players[i], hand));

        int found = 0;
        for (int j = 0; j < MAX_GAME_PLAYERS; j++) {
            if (hand->players[i] == players[i])
                found++;
        }
        cut_assert_equal_int(found, 0);
    }
}

/**
 * The function creates a hand where the players put the cards given as
 * parameters in order and compares it with the winner id provided.
 *
 * cardSuits and cardValue MUST have at least testSize slots and testSize
 * should be at least 2.
 */
void perform_round_handWinner_tests(int *cardSuits, int *cardValues, 
                                    enum Suit trump, int testSize, int winner)
{
    struct Hand *hand = round_createHand();
    struct Card **cards = malloc(testSize * sizeof(struct Card));
    struct Player **players = malloc(testSize * sizeof(struct Player));

    for (int i = 0; i < testSize; i++) {
        cards[i] = deck_createCard(cardSuits[i], cardValues[i]);
        players[i] = team_createPlayer("name", 0, 0);
        players[i]->hand[0] = cards[i]; //use specialised function for this
        round_addPlayer(players[i], hand);
        round_putCard(players[i], 0, hand);
    }

    cut_assert_equal_pointer(players[winner], round_handWinner(hand, trump));

    for (int i = 0; i < testSize; i++) {
        deck_deleteCard(&cards[i]);
        team_deletePlayer(&players[i]);
    }

    round_deleteHand(&hand);
    free(cards);
    free(players);
}


void test_round_handWinner()
{
    struct Hand *hand = round_createHand();
    cut_assert_equal_pointer(NULL, round_handWinner(NULL, CLUBS));
    cut_assert_equal_pointer(NULL, round_handWinner(NULL, SuitEnd));
    cut_assert_equal_pointer(NULL, round_handWinner(hand, SuitEnd));
    cut_assert_equal_pointer(NULL, round_handWinner(hand, CLUBS));
    
    struct Player *player1 = team_createPlayer("A", 0, 0);
    struct Player *player2 = team_createPlayer("A", 0, 0);

    struct Card *card1 = deck_createCard(CLUBS, VALUES[0]);

    player1->hand[0] = card1;

    round_addPlayer(player1, hand);
    round_putCard(player1, 0, hand);

    cut_assert_equal_pointer(NULL, round_handWinner(hand, DIAMONDS));
    //only one player

    round_addPlayer(player2, hand);
    cut_assert_equal_pointer(NULL, round_handWinner(hand, DIAMONDS));
    //player without card

    deck_deleteCard(&card1);
    team_deletePlayer(&player1);
    team_deletePlayer(&player2);
    round_deleteHand(&hand);


    //REAL-GAME TESTS
    int cardSuits[MAX_GAME_PLAYERS];
    int cardValues[MAX_GAME_PLAYERS];

    cardSuits[0] = CLUBS;
    cardSuits[1] = DIAMONDS;
    cardSuits[2] = CLUBS;
    cardSuits[3] = SPADES;

    cardValues[0] = VALUES[0];
    cardValues[1] = VALUES[1];
    cardValues[2] = VALUES[2];
    cardValues[3] = VALUES[3];

    perform_round_handWinner_tests(cardSuits, cardValues, DIAMONDS, 2, 1);
    perform_round_handWinner_tests(cardSuits, cardValues, SPADES, 2, 0);
    perform_round_handWinner_tests(cardSuits, cardValues, SPADES, 3, 2);
    perform_round_handWinner_tests(cardSuits, cardValues, SPADES, 4, 3);
    perform_round_handWinner_tests(cardSuits, cardValues, CLUBS, 4, 2);

    cardSuits[0] = CLUBS;
    cardSuits[1] = DIAMONDS;
    cardSuits[2] = CLUBS;
    cardSuits[3] = DIAMONDS;

    cardValues[0] = VALUES[2];
    cardValues[1] = VALUES[1];
    cardValues[2] = VALUES[0];
    cardValues[3] = VALUES[3];

    perform_round_handWinner_tests(cardSuits, cardValues, CLUBS, 4, 0);
    perform_round_handWinner_tests(cardSuits, cardValues, DIAMONDS, 4, 1);
}

