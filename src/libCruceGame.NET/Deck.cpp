#include "NETDeck.h"
#include "NETReferenceCounter.h"
#include "deck.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Deck::Deck() : CruceGame::Deck::Deck(IntPtr(deck_createDeck()))
{
}

CruceGame::Deck::Deck(IntPtr handle) : _deck(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Deck::~Deck()
{
    this->!Deck();
}

CruceGame::Deck::!Deck()
{
    ReferenceCounter::Instance->Subtract(_deck);
    if (ReferenceCounter::Instance[_deck] > 0)
        return;

    ::Deck* deck = static_cast<::Deck*>(_deck.ToPointer());

    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (deck->cards[i] != (void*)0)
        {
            ReferenceCounter::Instance->Subtract(IntPtr(deck->cards[i]));
        }
    }

    deck_deleteDeck(&deck);
}

IntPtr CruceGame::Deck::Handle::get()
{
    return _deck;
}

array<CruceGame::Card^>^ CruceGame::Deck::Cards::get()
{
    List<Card^> result;
    ::Deck* deck = static_cast<::Deck*>(_deck.ToPointer());

    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (deck->cards[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Card(IntPtr(deck->cards[i])));
        }
    }

    return result.ToArray();
}

int CruceGame::Deck::CardsCount::get()
{
    ::Deck* deck = static_cast<::Deck*>(_deck.ToPointer());
    return deck_cardsNumber(deck);
}

void CruceGame::Deck::Shuffle()
{
    ::Deck* deck = static_cast<::Deck*>(_deck.ToPointer());
    deck_deckShuffle(deck);
}

int CruceGame::Deck::GetHashCode()
{
    return (int)_deck.ToPointer();
}

bool CruceGame::Deck::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Deck^>(other));
}

bool CruceGame::Deck::Equals(CruceGame::Deck^ other)
{
    if (other == nullptr)
        return false;

    return _deck == other->_deck;
}