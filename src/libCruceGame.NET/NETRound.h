#pragma once

#include "NETDeck.h"
#include "NETPlayer.h"
#include "NETTeam.h"
#include "NETHand.h"

namespace CruceGame
{
    /// <summary>
    /// Represents a round
    /// </summary>
    public ref class Round sealed : System::IEquatable<Round^>
    {
    private:
        System::IntPtr _round;
    public:
        /// <summary>
        /// Creates a new round
        /// </summary>
        Round();
        /// <summary>
        /// Creates a new round wrapper from an existing handle
        /// </summary>
        Round(System::IntPtr handle);
        /// <summary>
        /// Destroys the round
        /// </summary>
        ~Round();
        /// <summary>
        /// Destroys the round
        /// </summary>
        !Round();
        /// <summary>
        /// Gets a handle to the unmanaged memory of the round
        /// </summary>
        property System::IntPtr Handle
        {
            System::IntPtr get();
        }
        /// <summary>
        /// Gets or sets the trump of the round
        /// </summary>
        property Suit Trump
        {
            Suit get();
            void set(Suit value);
        }
        /// <summary>
        /// Gets the hands of the round
        /// </summary>
        property array<Hand^>^ Hands
        {
            array<Hand^>^ get();
        }
        /// <summary>
        /// Gets the players in the round
        /// </summary>
        property array<Player^>^ Players
        {
            array<Player^>^ get();
        }
        /// <summary>
        /// Adds a player to the round
        /// </summary>
        /// <param name="player"></param>
        void AddPlayer(Player^ player);
        /// <summary>
        /// Removes a player from the round
        /// </summary>
        /// <param name="player"></param>
        void RemovePlayer(Player^ player);
        /// <summary>
        /// Distributes a deck to all the players in the round
        /// </summary>
        /// <param name="deck"></param>
        void DistributeDeck(Deck^ deck);
        /// <summary>
        /// Distributes a card from the deck to each player
        /// </summary>
        /// <param name="deck"></param>
        void DistributeOneCardToEachPlayer(Deck^ deck);
        /// <summary>
        /// Places a bid for a player
        /// </summary>
        /// <param name="player"></param>
        /// <param name="bid"></param>
        void PlaceBid(Player^ player, int bid);
        /// <summary>
        /// Gets the bid of a player
        /// </summary>
        /// <param name="player"></param>
        /// <returns></returns>
        int GetBid(Player^ player);
        /// <summary>
        /// Gets the winner of the bid
        /// </summary>
        /// <returns></returns>
        Player^ GetBidWinner();
        /// <summary>
        /// Places a card of a player in a hand
        /// </summary>
        /// <param name="hand"></param>
        /// <param name="player"></param>
        /// <param name="card"></param>
        void PlaceCard(Hand^ hand, Player^ player, Card^ card);
        /// <summary>
        /// Gets the winner of a hand
        /// </summary>
        /// <param name="hand"></param>
        /// <returns></returns>
        Player^ GetHandWinner(Hand^ hand);
        /// <summary>
        /// Gets the points of a player
        /// </summary>
        /// <param name="player"></param>
        int GetPoints(Player^ player);
        /// <summary>
        /// Gets the points of a team
        /// </summary>
        /// <param name="team"></param>
        int GetPoints(Team^ team);

        int GetHashCode() override;
        bool Equals(Object^ other) override;
        virtual bool Equals(Round^ other);
    };
}