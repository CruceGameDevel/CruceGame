#include "output.h"
#include <errors.h>

#include <wchar.h>
#include <locale.h>

int printCard(FILE *f, struct Card *card)
{
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
    fwprintf(f, L" ___ \n|%c  |\n|%lc  |\n|  %lc|\n|  %c|\n ~~~ ", value, suit, suit, value);
    fseek(f, 31, SEEK_SET);

    return NO_ERROR;
}
