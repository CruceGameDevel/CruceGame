#include "errors.h"

const char *error(int error_code)
{
    switch(error_code) {

        case NO_ERROR:
            return "There is no error";
        break;

        case POINTER_NULL:
            return "The pointer you passed points to NULL";
        break;
        case MALLOC_ERROR:
            return "malloc failed to allocate the requested block of memory";
        break;

        case CARD_NULL:
            return "The pointers to the card you are trying to access is NULL";
        break;
        case DECK_NULL:
            return "The pointer to the deck you passed as parameter is NULL";
        break;
        case PLAYER_NULL:
            return "The pointer to the player you passed as parameter is NULL";
        break;
        case TEAM_NULL:
            return "The pointer to the team you passed as paramater is NULL";
        break;

        case TEAM_FULL:
            return "You are trying to add a player to a team that has no more free places";
        break;
        case TEAM_EMPTY:
            return "You are trying to operate on a team that has no players";
        break;

        case DUPLICATE:
            return "You are trying to add a player or card that already exists";
        break;
        case NOT_FOUND:
            return "You are trying to operate on an item that has not been found";
        break;

        case ROUND_NULL:
            return "The pointer to the round you passed as parameter is NULL";
        break;
        case HAND_NULL:
            return "The pointer to the hand you passed as parameter is NULL";
        break;

        case ILLEGAL_VALUE:
            return "The value you try to operate is not allowed in context";
        break;

        case FULL:
            return "You are trying to add a player to a round or hand that are full";
        break;
        case ERROR_COMPARE:
            return "The values or suits of the cards you are trying to compare are illegal in the context";
        break;

        case HAND_EMPTY:
            return "Mai este folosit?";
        break;
        case DECK_EMPTY:
            return "Trying to distribute cards from a deck not large enough";
        break;

        case LESS_PLAYERS:
            return "The number of players needed is not big enough in the context given";
        break;
        case LESS_CARDS:
            return "There are not enough cards to complete the operation in the context given";
        break;

        case GAME_NULL:
            return "The pointer to the game structure you are trying to operate is null";
        break;
        case ROUND_EMPTY:
            return "There are not enough players in the round structure to complete the operations in the context given";
        break;
        case GAME_EMPTY:
            return "There are no players in the game structure; the minimum to complete the operations is two.";
        break;
        
        default:
            return "Unknown error code";
    }
}
