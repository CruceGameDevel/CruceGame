#include <libCruceGame.h>
#include "output.h"

#include <wchar.h>
#include <locale.h>

#define MAX_CARDS_PER_LINE 8

int printCard(FILE *f, struct Card *card, int position)
{
    if (position % MAX_CARDS_PER_LINE == 0) {
       fwprintf(f, L"\n\n\n\n\n\n");
    }
    fwprintf(f, L"\x1b[5A");
    wchar_t suit;
    switch (card->suit) {
        case DIAMONDS:
            suit = L'\u2666';
            break;
        case CLUBS:
            suit = L'\u2663';
            break;
        case HEARTS:
            suit = L'\u2665';
            break;
        case SPADES:
            suit = L'\u2660';
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

    setlocale(LC_ALL, "");
    fwprintf(f, L" ___ \x1b[B\x1b[5D");
    fwprintf(f, L"|%lc  |\x1b[B\x1b[5D", value);
    fwprintf(f, L"|%lc  |\x1b[B\x1b[5D", suit);
    fwprintf(f, L"|  %lc|\x1b[B\x1b[5D", suit);
    fwprintf(f, L"|  %lc|\x1b[B\x1b[5D", value);
    fwprintf(f, L" \u203e\u203e\u203e");

    return NO_ERROR;
}

int printPlayerCards(struct Player *player)
{
    if (player == NULL)
        return PLAYER_NULL;

    for (int i = 0; i < MAX_CARDS; i++) {
        if (player->hand[i] != NULL)
            printCard(stdout, player->hand[i], i);

    }
    return NO_ERROR;
}
