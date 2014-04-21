#pragma once

#include "NETPlayer.h"
#include "NETTeam.h"
#include "NETCard.h"
#include "NETHand.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a game
    /// </summary>
    public ref class Game sealed : System::IEquatable<Game^>
    {
    private:
        System::IntPtr _game;
    public:
        /// <summary>
        /// Creates a new game wrapper from an existing handle
        /// </summary>
        /// <param name="handle"></param>
        Game(System::IntPtr handle);
        /// <summary>
        /// Creates a new game with the specified ammount of total points
        /// </summary>
        /// <param name="points"></param>
        /// <returns>An instance of the new game or NULL on failure</returns>
        static Game^ Create(int points);
        /// <summary>
        /// Destroys the game
        /// </summary>
        ~Game();
        /// <summary>
        /// Destroys the game
        /// </summary>
        !Game();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the game
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the ammount of players in the game
        /// </summary>
        property int PlayersCount
        {
            int get();
        }
        /// <summary>
        /// Gets the total points necessary to win the game
        /// </summary>
        property int TotalPoints
        {
            int get();
        }
        /// <summary>
        /// Gets the players in the game
        /// </summary>
        property array<Player^>^ Players
        {
            array<Player^>^ get();
        }
        /// <summary>
        /// Gets the teams in the game
        /// </summary>
        property array<Team^>^ Teams
        {
            array<Team^>^ get();
        }
        /// <summary>
        /// Adds a player to the game
        /// </summary>
        /// <param name="player"></param>
        void AddPlayer(Player^ player);
        /// <summary>
        /// Removes a player from the game
        /// </summary>
        /// <param name="player"></param>
        void RemovePlayer(Player^ player);
        /// <summary>
        /// Adds a team to the game
        /// </summary>
        /// <param name="team"></param>
        void AddTeam(Team^ team);
        /// <summary>
        /// Removes a team from the game
        /// </summary>
        /// <param name="team"></param>
        void RemoveTeam(Team^ team);
        /// <summary>
        /// Gets the winning team
        /// </summary>
        /// <returns></returns>
        Team^ GetWinningTeam();
        /// <summary>
        /// Finds the team that a player belongs to
        /// </summary>
        /// <param name="player"></param>
        /// <returns>The team that the player belongs to or NULL on failure</returns>
        Team^ FindTeam(Player^ player);
        /// <summary>
        /// Checks if a card can be played in the game
        /// </summary>
        /// <param name="hand">Current hand</param>
        /// <param name="player">Player that wants to play the card</param>
        /// <param name="card">Card to be played</param>
        /// <returns>TRUE if the card can be played</returns>
        bool CheckCard(Hand^ hand, Player^ player, Card^ card);
        /// <summary>
        /// Updates the score
        /// </summary>
        /// <param name="bidWinner"></param>
        void UpdateScore(Player^ bidWinner);
        /// <summary>
        /// Starts a new round
        /// </summary>
        /// <param name="firstInRound">Player to be the first to play in the round</param>
        void StartRound(Player^ firstInRound);

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Game^ other);
    };
}