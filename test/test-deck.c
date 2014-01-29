#include <deck.h>
#include <constants.h>
#include <errors.h>

#include <cutter.h>
#include <string.h>

#include <stdio.h>

void test_deck_createCard()
{
    struct Card *card;
    cut_assert_equal_pointer(NULL, deck_createCard(SuitEnd, VALUES[0]));
    cut_assert_equal_pointer(NULL, deck_createCard(DIAMONDS, 100));
    for (int i = 1; i < SuitEnd; i++) {
        for (int j = 0; VALUES[j] != -1; j++) {
            card = deck_createCard(i, VALUES[j]);
            cut_assert_equal_int(card->suit, i);
            cut_assert_equal_int(card->value, VALUES[j]);
        }
    }
}

void test_deck_deleteCard()
{
    struct Card *card = deck_createCard(1, VALUES[0]);
    cut_assert_equal_int(NO_ERROR, deck_deleteCard(&card));
    cut_assert_equal_int(CARD_NULL, deck_deleteCard(&card));
    cut_assert_equal_int(POINTER_NULL, deck_deleteCard(NULL));
}

int cardsEqual(struct Card a, struct Card b)
{
    if (a.suit == b.suit && a.value == b.value) {
        return 1;
    }
    return 0;
}

void test_deck_deleteDeck()
{
    struct Deck *deck = deck_createDeck();

    deck_deleteDeck(&deck);
    cut_assert_equal_pointer(deck, NULL);
}

void test_deck_createDeck()
{
    struct Deck *deck = deck_createDeck();

    int duplicates = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            if (i != j) {
                duplicates += cardsEqual(deck->cards[i], deck->cards[j]);
            }
        }
    }
    cut_assert_equal_int(duplicates, 0);

    deck_deleteDeck(&deck);
}

void test_deck_deckShuffle()
{
    struct Deck *deck = deck_createDeck();
    struct Deck *shuffled = malloc(sizeof(struct Deck));
    memcpy(shuffled, deck, sizeof(struct Deck));

    cut_assert_equal_int(0, deck_deckShuffle(shuffled));

    int differences = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (!cardsEqual(shuffled->cards[i], deck->cards[i])) {
            differences++;
        }
    }

    cut_assert_not_equal_int(differences, 0);

    deck_deleteDeck(&deck);
    deck_deleteDeck(&shuffled);
}

void test_extensive_deck_deckShuffle()
{
    for (int i = 0; i < 100; i++) {
        test_deck_deckShuffle();
    }
}

void test_deck_compareCards()
{
    struct Card *card1 = malloc(sizeof(struct Card));
    struct Card *card2 = malloc(sizeof(struct Card));
    enum Suit tromf = DIAMONDS;

    cut_assert_equal_int(CARD_NULL, deck_compareCards(NULL, NULL, tromf));
    cut_assert_equal_int(CARD_NULL, deck_compareCards(NULL, card2, tromf));
    cut_assert_equal_int(CARD_NULL, deck_compareCards(card1, NULL, tromf));
    
    card1->suit = SuitEnd;
    card2->suit = CLUBS;
    cut_assert_equal_int(ILLEGAL_VALUE,
                         deck_compareCards(card1, card2, tromf));
    cut_assert_equal_int(ILLEGAL_VALUE,
                         deck_compareCards(card2, card1, tromf));
    
    card1->suit = CLUBS;
    card1->value = VALUES[0];
    card2->value = -1;
    cut_assert_equal_int(ILLEGAL_VALUE,
                         deck_compareCards(card1, card2, tromf));
    cut_assert_equal_int(ILLEGAL_VALUE,
                         deck_compareCards(card2, card1, tromf));

    cut_assert_equal_int(0, deck_compareCards(card1, card1, tromf));

    card2->value = VALUES[1];
    card2->suit = DIAMONDS;
    cut_assert_equal_int(1, deck_compareCards(card2, card1, tromf));
    cut_assert_equal_int(2, deck_compareCards(card1, card2, tromf));

    card2->suit = SPADES;
    cut_assert_equal_int(1, deck_compareCards(card1, card2, tromf));
    cut_assert_equal_int(1, deck_compareCards(card2, card1, tromf));

    card2->suit = CLUBS;
    cut_assert_equal_int(1, deck_compareCards(card2, card1, tromf));
    cut_assert_equal_int(2, deck_compareCards(card1, card2, tromf));

    tromf = CLUBS;
    cut_assert_equal_int(1, deck_compareCards(card2, card1, tromf));
    cut_assert_equal_int(2, deck_compareCards(card1, card2, tromf));

    free(card1);
    card1 = NULL;
    free(card2);
    card2 = NULL;
}
