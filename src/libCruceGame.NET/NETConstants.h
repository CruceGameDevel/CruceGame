#pragma once

#include "constants.h"

namespace CruceGame
{
    /// <summary>
    /// Stores game-related constants
    /// </summary>
    public ref class Constants sealed
    {
    public:
        /// <summary>
        /// Stores the maximum number of cards in a game
        /// </summary>
        static const int MaxCards       = MAX_CARDS;
        /// <summary>
        /// Stores the maximum number of players in a team
        /// </summary>
        static const int MaxTeamPlayers = MAX_TEAM_PLAYERS;
        /// <summary>
        /// Stores the maximum number of cards in a deck
        /// </summary>
        static const int DeckSize       = DECK_SIZE;
        static const int SwapMin        = SWAP_MIN;
        static const int SwapMax        = SWAP_MAX;
        /// <summary>
        /// Stores the maximum number of hands
        /// </summary>
        static const int MaxHands = MAX_HANDS;
        /// <summary>
        /// Stores the maximum number of players in a game
        /// </summary>
        static const int MaxGamePlayers = MAX_GAME_PLAYERS;
        /// <summary>
        /// Stores the maximum number of teams in a game
        /// </summary>
        static const int MaxGameTeams = MAX_GAME_TEAMS;
        /// <summary>
        /// Stores the numeric value of each card
        /// </summary>
        static initonly array<int>^ CardValues = gcnew array<int>
        {
            VALUES[0], VALUES[1], VALUES[2], VALUES[3], VALUES[4], VALUES[5]
        };
    };
    /// <summary>
    /// The suit of a card
    /// </summary>
    public enum class Suit
    {
        Diamonds    = DIAMONDS,
        Clubs       = CLUBS,
        Spades      = SPADES,
        Hearts      = HEARTS
    };
}