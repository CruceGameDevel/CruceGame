#include "NETPlayer.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "team.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Player^ CruceGame::Player::Create(String^ name)
{
    IntPtr _name = CruceGame::Helpers::ManagedStringToUnmanaged(name);
    auto player = team_createPlayer(static_cast<char*>(_name.ToPointer()), 1);
    CruceGame::Helpers::FreeUnmanagedString(_name);
    if (player == (void*)0)
        return nullptr;

    return gcnew Player(IntPtr(player));
}

CruceGame::Player::Player(IntPtr handle) : _player(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Player::~Player()
{
    this->!Player();
}

CruceGame::Player::!Player()
{
    ReferenceCounter::Instance->Subtract(_player);
    if (ReferenceCounter::Instance[_player] > 0)
        return;

    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    team_deletePlayer(&player);
}

IntPtr CruceGame::Player::Handle::get()
{
    return _player;
}

int CruceGame::Player::Score::get()
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    return player->score;
}

String^ CruceGame::Player::Name::get()
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    return CruceGame::Helpers::UnmanagedStringToManaged(IntPtr((void*)player->name));
}

array<CruceGame::Card^>^ CruceGame::Player::Hand::get()
{
    List<CruceGame::Card^> result;
    ::Player* player = static_cast<::Player*>(_player.ToPointer());

    for (int i = 0; i < MAX_CARDS; i++)
    {
        if (player->hand[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Card(IntPtr(player->hand[i])));
        }
    }

    return result.ToArray();
}

void CruceGame::Player::Score::set(int value)
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    player->score = value;
}

bool CruceGame::Player::IsHuman::get()
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    return player->isHuman != 0;
}

bool CruceGame::Player::HasCards::get()
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    return team_hasCards(player) != 0;
}

void CruceGame::Player::AddCard(CruceGame::Card^ card)
{
    ::Player* player = static_cast<::Player*>(_player.ToPointer());
    ::Card* _card = static_cast<::Card*>(card->Handle.ToPointer());

    CheckErrorCode(team_addCard(player, _card));
}

String^ CruceGame::Player::ToString()
{
    return String::Format("#{0}", Name);
}

int CruceGame::Player::GetHashCode()
{
    return (int)_player.ToPointer();
}

bool CruceGame::Player::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Player^>(other));
}

bool CruceGame::Player::Equals(CruceGame::Player^ other)
{
    if (other == nullptr)
        return false;

    return _player == other->_player;
}