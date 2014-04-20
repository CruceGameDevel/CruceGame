#pragma once

#include "NETCard.h"
#include "NETPlayer.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a hand
    /// </summary>
    public ref class Hand sealed : System::IEquatable<Hand^>
    {
    private:
        System::IntPtr _hand;
    public:
        /// <summary>
        /// Creates a new hand
        /// </summary>
        Hand();
        /// <summary>
        /// Creates a new hand wrapper fron an existing handle
        /// </summary>
        Hand(System::IntPtr handle);
        /// <summary>
        /// Destroys the hand
        /// </summary>
        ~Hand();
        /// <summary>
        /// Destroys the hand
        /// </summary>
        !Hand();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the hand
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the cards in the hand
        /// </summary>
        property array<CruceGame::Card^>^ Cards
        {
            array<CruceGame::Card^>^ get();
        }
        /// <summary>
        /// Gets the players in the hand
        /// </summary>
        property array<CruceGame::Player^>^ Players
        {
            array<CruceGame::Player^>^ get();
        }
        /// <summary>
        /// Adds a player to the hand
        /// </summary>
        /// <param name="player"></param>
        void AddPlayer(Player^ player);
        /// <summary>
        /// Removes a player from the hand
        /// </summary>
        /// <param name="player"></param>
        void RemovePlayer(Player^ player);

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Hand^ other);
    };
}