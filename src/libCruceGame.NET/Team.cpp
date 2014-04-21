#include "NETTeam.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "team.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Team^ CruceGame::Team::Create()
{
    auto team = team_createTeam();
    if (team == (void*)0)
        return nullptr;

    return gcnew Team(IntPtr(team));
}

CruceGame::Team::Team(IntPtr handle) : _team(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Team::~Team()
{
    this->!Team();
}

CruceGame::Team::!Team()
{
    ReferenceCounter::Instance->Subtract(_team);
    if (ReferenceCounter::Instance[_team] > 0)
        return;

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
    {
        ::Team* team = static_cast<::Team*>(_team.ToPointer());
        if (team->players[i] != (void*)0)
        {
            IntPtr handle = IntPtr(team->players[i]);
            ReferenceCounter::Instance->Subtract(handle);
            if (ReferenceCounter::Instance[handle] < 1)
            {
                //this is the only reference of the player left
                team_deletePlayer(&team->players[i]);
            }
        }
    }

    ::Team* team = static_cast<::Team*>(_team.ToPointer());
    team_deleteTeam(&team);
}

IntPtr CruceGame::Team::Handle::get()
{
    return _team;
}

array<CruceGame::Player^>^ CruceGame::Team::Players::get()
{
    List<CruceGame::Player^> result;
    ::Team* team = static_cast<::Team*>(_team.ToPointer());

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
    {
        if (team->players[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Player(IntPtr(team->players[i])));
        }
    }

    return result.ToArray();
}

int CruceGame::Team::Score::get()
{
    ::Team* team = static_cast<::Team*>(_team.ToPointer());
    return team->score;
}

void CruceGame::Team::AddPlayer(CruceGame::Player^ player)
{
    ::Team* team = static_cast<::Team*>(_team.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    ReferenceCounter::Instance->Add(player->Handle);
    CheckErrorCode(team_addPlayer(team, _player));
}

void CruceGame::Team::RemovePlayer(CruceGame::Player^ player)
{
    ::Team* team = static_cast<::Team*>(_team.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    ReferenceCounter::Instance->Remove(player->Handle);
    CheckErrorCode(team_removePlayer(team, _player));
}

void CruceGame::Team::UpdatePlayersScore()
{
    ::Team* team = static_cast<::Team*>(_team.ToPointer());
    CheckErrorCode(team_updatePlayersScore(team));
}

int CruceGame::Team::GetHashCode()
{
    return (int)_team.ToPointer();
}

bool CruceGame::Team::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Team^>(other));
}

bool CruceGame::Team::Equals(CruceGame::Team^ other)
{
    if (other == nullptr)
        return false;

    return _team == other->_team;
}