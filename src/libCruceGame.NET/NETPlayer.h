#pragma once

#include "NETCard.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a player
    /// </summary>
    public ref class Player : System::IEquatable<Player^>
    {
    protected:
        System::IntPtr _player;
    public:
        /// <summary>
        /// Creates a new player wrapper from an existing handle
        /// </summary>
        Player(System::IntPtr handle);
        /// <summary>
        /// Creates a new player with a unique name
        /// </summary>
        /// <param name="name"></param>
        /// <returns>The new player or NULL on failure</returns>
        static Player^ Create(System::String^ name);
        /// <summary>
        /// Destroys the player
        /// </summary>
        ~Player();
        /// <summary>
        /// Destroys the player
        /// </summary>
        !Player();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the player
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the name of the player
        /// </summary>
        property System::String^ Name
        {
            System::String^ get();
        }
        /// <summary>
        /// Gets the current hand of the player
        /// </summary>
        property array<CruceGame::Card^>^ Hand
        {
            array<CruceGame::Card^>^ get();
        }
        /// <summary>
        /// Gets or sets the score of the player
        /// </summary>
        property int Score
        {
            int get();
            void set(int value);
        }
        /// <summary>
        /// Gets whether the player is human
        /// </summary>
        property bool IsHuman
        {
            bool get();
        }
        /// <summary>
        /// Gets whether the player has any cards
        /// </summary>
        property bool HasCards
        {
            bool get();
        }
        /// <summary>
        /// Adds a card to the hand of the player
        /// </summary>
        /// <param name="card"></param>
        void AddCard(CruceGame::Card^ card);       
        /// <summary>
        /// Creates a string representation of the player
        /// </summary>
        System::String^ ToString() override;

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Player^ other);
    };
}