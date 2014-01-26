#include <team.h>
#include <cutter.h>

static struct Team team;

void test_team_createPlayer()
{
    struct Player *player;
    for (int i = 0; i < 100; i++) {
        player = team_createPlayer("A", i, i);
        cut_assert_equal_string("A", player->name);
        cut_assert_equal_int(i, player->sockfd);
        cut_assert_equal_int(i, player->isHuman);
        cut_assert_equal_int(0, player->score);
        cut_assert_operator_int(-1, <, player->id);
    }
}

void test_team_createTeam()
{
    struct Team *team;
    for (int i = 0; i < 100; i++) {
        team = team_createTeam("A");
        cut_assert_equal_string("A", team->name);
        cut_assert_operator_int(-1, <, team->id);
    }
}

void test_team_addPlayer()
{
    struct Player *player1 = NULL;
    struct Player *player2 = NULL;
    struct Player *player3 = NULL;
    struct Team *team1     = NULL;
    int maxPlayers = MAX_PLAYERS;
    int i;
    int playerAdded; //0 the player was added. Otherwise -1.
    
    cut_assert_equal_int(-1, team_addPlayer(NULL, NULL));
    
    team1 = team_createTeam("E");
    cut_assert_equal_int(-1, team_addPlayer(team1, NULL));
    
    player1 = team_createPlayer("A", 0, 0);
    cut_assert_equal_int(-1, team_addPlayer(NULL, player1));
    
    team1 = team_createTeam("E");
    player1 = team_createPlayer("A", 0, 0);
    cut_assert_operator_int(maxPlayers, >, team_addPlayer(team1, player1));
    maxPlayers--;
    
    playerAdded = -1;
    for (i = 0; i < MAX_PLAYERS; i++)
        if (team1->players[i] == player1)
            playerAdded = 0;
    cut_assert_equal_int(playerAdded, 0);
    cut_assert_equal_int(-1, team_addPlayer(team1, player1));
    
    player2 = team_createPlayer("B", 0, 0);
    player3 = team_createPlayer("C", 0, 0);
    cut_assert_operator_int(maxPlayers, >, team_addPlayer(team1, player2));
    maxPlayers--;
    
    playerAdded = -1;
    for (i = 0; i < MAX_PLAYERS; i++)
        if (team1->players[i] == player1)
            playerAdded = 0;
    cut_assert_equal_int(playerAdded, 0);
    
    cut_assert_operator_int(maxPlayers, >, team_addPlayer(team1, player3));
}