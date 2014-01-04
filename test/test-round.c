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
    hand = malloc(sizeof(struct Hand));
    hand->players[0] = NULL;

    player1 = malloc(sizeof(struct Player));
    player2 = malloc(sizeof(struct Player));
    player3 = malloc(sizeof(struct Player));
    player4 = malloc(sizeof(struct Player));
}

void cut_teardown()
{
    free(hand);
    free(player1);
    free(player2);
    free(player3);
    free(player4);
}

