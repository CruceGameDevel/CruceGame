#include "NETRound.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "round.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Round::Round() : CruceGame::Round::Round(IntPtr(round_createRound()))
{
}

CruceGame::Round::Round(IntPtr handle) : _round(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Round::~Round()
{
    this->!Round();
}

CruceGame::Round::!Round()
{
    ReferenceCounter::Instance->Subtract(_round);
    if (ReferenceCounter::Instance[_round] > 0)
        return;

    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    round_deleteRound(&round);
}

IntPtr CruceGame::Round::Handle::get()
{
    return _round;
}

CruceGame::Suit CruceGame::Round::Trump::get()
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    return (CruceGame::Suit)round->trump;
}

System::Void CruceGame::Round::Trump::set(CruceGame::Suit value)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    round->trump = (::Suit)value;
}

array<CruceGame::Hand^>^ CruceGame::Round::Hands::get()
{
    List<CruceGame::Hand^> result;
    ::Round* round = static_cast<::Round*>(_round.ToPointer());

    for (int i = 0; i < MAX_HANDS; i++)
    {
        if (round->hands[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Hand(IntPtr(round->hands[i])));
        }
    }

    return result.ToArray();
}

array<CruceGame::Player^>^ CruceGame::Round::Players::get()
{
    List<CruceGame::Player^> result;
    ::Round* round = static_cast<::Round*>(_round.ToPointer());

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        if (round->players[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Player(IntPtr(round->players[i])));
        }
    }

    return result.ToArray();
}

void CruceGame::Round::AddPlayer(CruceGame::Player^ player)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(round_addPlayer(_player, round));
    ReferenceCounter::Instance->Add(player->Handle);
}

void CruceGame::Round::RemovePlayer(CruceGame::Player^ player)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(round_removePlayer(_player, round));
    ReferenceCounter::Instance->Remove(player->Handle);
}

System::Void CruceGame::Round::DistributeDeck(CruceGame::Deck^ deck)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Deck* _deck = static_cast<::Deck*>(deck->Handle.ToPointer());

    round_distributeDeck(_deck, round);
}

System::Void CruceGame::Round::DistributeOneCardToEachPlayer(CruceGame::Deck^ _deck)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Deck* deck = static_cast<::Deck*>(_deck->Handle.ToPointer());

    round_distributeCard(deck, round);
}

void CruceGame::Round::PlaceBid(CruceGame::Player^ player, int bid)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(round_placeBid(_player, bid, round));
}

int CruceGame::Round::GetBid(CruceGame::Player^ player)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());

    int playerId = round_findPlayerIndexRound(_player, round);

    if (playerId < 0)
        CheckErrorCode(playerId);

    return round->bids[playerId];
}

CruceGame::Player^ CruceGame::Round::GetBidWinner()
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = round_getBidWinner(round);

    if (_player == (void*)0)
    {
        return nullptr;
    }

    //place the bid winner the first to place cards in the next round;
    round_arrangePlayersHand(round, round_findPlayerIndexRound(_player, round));

    return gcnew Player(IntPtr(_player));
}

System::Void CruceGame::Round::PlaceCard(CruceGame::Hand^ hand, CruceGame::Player^ player, CruceGame::Card^ card)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Hand* _hand = static_cast<::Hand*>(hand->Handle.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    ::Card* _card = static_cast<::Card*>(card->Handle.ToPointer());

    int playerId = round_findPlayerIndexRound(_player, round);
    int handId = -1;
    int cardId = -1;

    for (int i = 0; i < MAX_CARDS; i++)
    {
        if (_player->hand[i] == _card)
        {
            cardId = i;
            break;
        }
    }

    if (cardId < 0)
        Helpers::GenerateErrorCodeException(CARD_NULL);

    for (int i = 0; i < MAX_HANDS; i++)
    {
        if (round->hands[i] == _hand)
        {
            handId = i;
            break;
        }
    }

    if (handId < 0)
        Helpers::GenerateErrorCodeException(HAND_NULL);
    
    CheckErrorCode(round_putCard(_player, cardId, handId, round));
}

CruceGame::Player^ CruceGame::Round::GetHandWinner(CruceGame::Hand^ hand)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Hand* _hand = static_cast<::Hand*>(hand->Handle.ToPointer());
    ::Player* _player = round_handWinner(_hand, round);

    if (_player == (void*)0)
    {
        return nullptr;
    }

    //place the hand winner the first to place cards in the next round;
    round_arrangePlayersHand(round, round_findPlayerIndexRound(_player, round));

    return gcnew Player(IntPtr(_player));
}

int CruceGame::Round::GetPoints(CruceGame::Player^ player)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());

    int playerId = round_findPlayerIndexRound(_player, round);

    if (playerId < 0)
        CheckErrorCode(playerId);

    return round->pointsNumber[playerId];
}

int CruceGame::Round::GetPoints(CruceGame::Team^ team)
{
    ::Round* round = static_cast<::Round*>(_round.ToPointer());
    ::Team* _team = static_cast<::Team*>(team->Handle.ToPointer());

    int result = round_computePoints(_team, round);

    if (result < 0)
        CheckErrorCode(result);

    return result;
}

int CruceGame::Round::GetHashCode()
{
    return (int)_round.ToPointer();
}

bool CruceGame::Round::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Round^>(other));
}

bool CruceGame::Round::Equals(CruceGame::Round^ other)
{
    if (other == nullptr)
        return false;

    return _round == other->_round;
}