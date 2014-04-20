#include "NETHand.h"
#include "NETPlayer.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "round.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Hand::Hand() : CruceGame::Hand::Hand(IntPtr(round_createHand()))
{
}

CruceGame::Hand::Hand(IntPtr handle) : _hand(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Hand::~Hand()
{
    this->!Hand();
}

CruceGame::Hand::!Hand()
{
    ReferenceCounter::Instance->Subtract(_hand);
    if (ReferenceCounter::Instance[_hand] > 0)
        return;
    
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());
        if (hand->players[i] != (void*)0)
        {
            IntPtr handle = IntPtr(hand->players[i]);
            ReferenceCounter::Instance->Subtract(handle);
            if (ReferenceCounter::Instance[handle] < 1)
            {
                //this is the only reference of the player left
                team_deletePlayer(&hand->players[i]);
            }
        }
    }

    ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());
    round_deleteHand(&hand);
}

IntPtr CruceGame::Hand::Handle::get()
{
    return _hand;
}

array<CruceGame::Card^>^ CruceGame::Hand::Cards::get()
{
    List<CruceGame::Card^> result;
    ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        if (hand->cards[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Card(IntPtr(hand->cards[i])));
        }
    }

    return result.ToArray();
}

array<CruceGame::Player^>^ CruceGame::Hand::Players::get()
{
    List<CruceGame::Player^> result;
    ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        if (hand->players[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Player(IntPtr(hand->players[i])));
        }
    }

    return result.ToArray();
}

void CruceGame::Hand::AddPlayer(CruceGame::Player^ player)
{
    ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(round_addPlayerHand(_player, hand));
    ReferenceCounter::Instance->Add(player->Handle);
}

void CruceGame::Hand::RemovePlayer(CruceGame::Player^ player)
{
    ::Hand* hand = static_cast<::Hand*>(_hand.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(round_removePlayerHand(_player, hand));
    ReferenceCounter::Instance->Remove(player->Handle);
}

int CruceGame::Hand::GetHashCode()
{
    return (int)_hand.ToPointer();
}

bool CruceGame::Hand::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Hand^>(other));
}

bool CruceGame::Hand::Equals(CruceGame::Hand^ other)
{
    if (other == nullptr)
        return false;

    return _hand == other->_hand;
}