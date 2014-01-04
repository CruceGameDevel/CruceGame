#include "round.h"
#include <stdlib.h>
#include <stdio.h>

struct Player *getBidWinner(struct Hand *hand)
{
    if (hand == NULL)
        return NULL;

    int maxBidIndex = 0;
    for (int i = 0; hand->players[i] != NULL; i++) {
        if (hand->bids[i] > hand->bids[maxBidIndex]) {
            maxBidIndex = i;
        }
    }
    return hand->players[maxBidIndex];
}

