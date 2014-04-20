#pragma once

#include "NETConstants.h"
#include "NETCard.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a deck of cards
    /// </summary>
    public ref class Deck sealed : System::IEquatable<Deck^>
    {
    private:
        System::IntPtr _deck;
    public:
        /// <summary>
        /// Creates a new deck
        /// </summary>
        Deck();
        /// <summary>
        /// Creates a new deck wrapper from an existing handle
        /// </summary>
        /// <param name="handle"></param>
        Deck(System::IntPtr handle);
        /// <summary>
        /// Destroys the deck
        /// </summary>
        ~Deck();
        /// <summary>
        /// Destroys the deck
        /// </summary>
        !Deck();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the deck
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the cards of the deck
        /// </summary>
        property array<CruceGame::Card^>^ Cards
        {
            array<CruceGame::Card^>^ get();
        }
        /// <summary>
        /// Gets the number of cards in the deck
        /// </summary>
        property int CardsCount
        {
            int get();
        }
        /// <summary>
        /// Shuffles the deck
        /// </summary>
        void Shuffle();

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Deck^ other);
    };
}