#pragma once

#include "NETPlayer.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a team
    /// </summary>
    public ref class Team : System::IEquatable<Team^>
    {
    protected:
        System::IntPtr _team;
    public:
        /// <summary>
        /// Creates a new team wrapper from an existing handle
        /// </summary>
        Team(System::IntPtr handle);
        /// <summary>
        /// Creates a new team
        /// </summary>
        /// <returns>A new team or NULL on error</returns>
        static Team^ Create();
        /// <summary>
        /// Destroys the team
        /// </summary>
        ~Team();
        /// <summary>
        /// Destroys the team
        /// </summary>
        !Team();
        /// <summary>
        /// Gets a handle to the unmananged memory of the team
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the players in the team
        /// </summary>
        property array<Player^>^ Players
        {
            array<Player^>^ get();
        }
        /// <summary>
        /// Gets the score of the team
        /// </summary>
        property int Score
        {
            int get();
        }
        /// <summary>
        /// Adds a player to the team
        /// </summary>
        /// <param name="player"></param>
        void AddPlayer(Player^ player);
        /// <summary>
        /// Removes a player from the team
        /// </summary>
        /// <param name="player"></param>
        void RemovePlayer(Player^ player);
        /// <summary>
        /// Updates the score of players
        /// </summary>
        void UpdatePlayersScore();

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Team^ other);
    };
}