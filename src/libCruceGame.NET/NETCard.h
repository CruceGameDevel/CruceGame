#pragma once

#include "NETConstants.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a card 
    /// </summary>
    public ref class Card sealed : System::IEquatable<Card^>
    {
    private:
        System::IntPtr _card;
    public:
        /// <summary>
        /// Creates a new card with the specified suit and value
        /// </summary>
        /// <param name="suit"></param>
        /// <param name="value"></param>
        static Card^ Create(Suit suit, int value);
    public:
        /// <summary>
        /// Creates a new card wrapper from an existing handle
        /// </summary>
        /// <param name="handle"></param>
        Card(System::IntPtr handle);
        /// <summary>
        /// Destroys the card
        /// </summary>
        ~Card();
        /// <summary>
        /// Destroys the card
        /// </summary>
        !Card();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the card
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets the card's suit
        /// </summary>
        property Suit Suit
        {
            CruceGame::Suit get();
        }
        /// <summary>
        /// Gets the card's value
        /// </summary>
        property int Value
        {
            int get();
        }
        /// <summary>
        /// Compares the card to another one based on a trump
        /// </summary>
        /// <returns>The more significant card</returns>
        /// <param name="second"></param>
        /// <param name="trump"></param>
        Card^ Compare(Card^ second, CruceGame::Suit trump);
        /// <summary>
        /// Creates a string representation of the card
        /// </summary>
        System::String^ ToString() override;

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Card^ other);
    };
}