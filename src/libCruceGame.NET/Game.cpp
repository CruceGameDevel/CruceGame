#include "NETGame.h"
#include "NETPlayer.h"
#include "NETHelpers.h"
#include "NETReferenceCounter.h"
#include "game.h"

using namespace System;
using namespace System::Collections::Generic;

CruceGame::Game::Game(IntPtr handle) : _game(handle)
{
    ReferenceCounter::Instance->Add(handle);
}

CruceGame::Game^ CruceGame::Game::Create(int points)
{
    auto game = game_createGame(points);
    if (game == (void*)0)
        return nullptr;

    return gcnew Game(IntPtr(game));
}


CruceGame::Game::~Game()
{
    this->!Game();
}

CruceGame::Game::!Game()
{
    ReferenceCounter::Instance->Subtract(_game);
    if (ReferenceCounter::Instance[_game] > 0)
        return;

    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    game_deleteGame(&game);
}

IntPtr CruceGame::Game::Handle::get()
{
    return _game;
}

int CruceGame::Game::PlayersCount::get()
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    return game->numberPlayers;
}

int CruceGame::Game::TotalPoints::get()
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    return game->pointsNumber;
}

array<CruceGame::Player^>^ CruceGame::Game::Players::get()
{
    List<CruceGame::Player^> result;
    ::Game* game = static_cast<::Game*>(_game.ToPointer());

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        if (game->players[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Player(IntPtr(game->players[i])));
        }
    }

    return result.ToArray();
}

array<CruceGame::Team^>^ CruceGame::Game::Teams::get()
{
    List<CruceGame::Team^> result;
    ::Game* game = static_cast<::Game*>(_game.ToPointer());

    for (int i = 0; i < MAX_GAME_TEAMS; i++)
    {
        if (game->teams[i] != (void*)0)
        {
            result.Add(gcnew CruceGame::Team(IntPtr(game->teams[i])));
        }
    }

    return result.ToArray();
}


void CruceGame::Game::AddPlayer(CruceGame::Player^ player)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(game_addPlayer(_player, game));
    ReferenceCounter::Instance->Add(player->Handle);
}

void CruceGame::Game::RemovePlayer(CruceGame::Player^ player)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    CheckErrorCode(game_removePlayer(_player, game));
    ReferenceCounter::Instance->Remove(player->Handle);
}

void CruceGame::Game::AddTeam(CruceGame::Team^ team)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Team* _team = static_cast<::Team*>(team->Handle.ToPointer());
    CheckErrorCode(game_addTeam(_team, game));
    ReferenceCounter::Instance->Add(team->Handle);
}

void CruceGame::Game::RemoveTeam(CruceGame::Team^ team)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Team* _team = static_cast<::Team*>(team->Handle.ToPointer());
    CheckErrorCode(game_removeTeam(_team, game));
    ReferenceCounter::Instance->Remove(team->Handle);
}

CruceGame::Team^ CruceGame::Game::GetWinningTeam()
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Team* team = game_winningTeam(game);

    if (team == (void*)0)
    {
        return nullptr;
    }

    return gcnew CruceGame::Team(IntPtr(team));
}

CruceGame::Team^ CruceGame::Game::FindTeam(CruceGame::Player^ player)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    ::Team* team = game_findTeam(game, _player);

    if (team == (void*)0)
    {
        return nullptr;
    }

    return gcnew CruceGame::Team(IntPtr(team));
}

bool CruceGame::Game::CheckCard(CruceGame::Hand^ hand, CruceGame::Player^ player, CruceGame::Card^ card)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Hand* _hand = static_cast<::Hand*>(hand->Handle.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());
    ::Card* _card = static_cast<::Card*>(card->Handle.ToPointer());

    int cardId = -1;

    for (int i = 0; i < MAX_CARDS; i++)
    {
        if (_player->hand[i] == _card)
        {
            cardId = i;
            break;
        }
    }
    
    int result = game_checkCard(_player, game, _hand, cardId);
    switch (result)
    {
    case 1:
        return true;
    case 0:
        return false;
    default:
        CheckErrorCode(result);
    }

    return false;
}

System::Void CruceGame::Game::UpdateScore(CruceGame::Player^ player)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());

    CheckErrorCode(game_updateScore(game, _player));
}

System::Void CruceGame::Game::StartRound(CruceGame::Player^ player)
{
    ::Game* game = static_cast<::Game*>(_game.ToPointer());
    ::Player* _player = static_cast<::Player*>(player->Handle.ToPointer());

    int playerId = -1;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
    {
        if (game->players[i] == _player)
        {
            playerId = i;
            break;
        }
    }

    CheckErrorCode(game_arrangePlayersRound(game, playerId));
}



int CruceGame::Game::GetHashCode()
{
    return (int)_game.ToPointer();
}

bool CruceGame::Game::Equals(Object^ other)
{
    return Equals(dynamic_cast<CruceGame::Game^>(other));
}

bool CruceGame::Game::Equals(CruceGame::Game^ other)
{
    if (other == nullptr)
        return false;

    return _game == other->_game;
}