#include <deck.h>
#include <stdio.h>
#include "output.h"

void printCard(struct Card *card)
{
    FILE *f = stderr;
    fprintf(stderr, " ___ \n|%d  |\n|%c  |\n|  %c|\n|  %d|\n ~~~ ", 6, 'a', 'a', 6);

}
