#include <team.h>
#include <errors.h>
#include <constants.h>

#include <cutter.h>

static struct Team team;

void test_team_createPlayer()
{
    cut_assert_equal_pointer(NULL, team_createPlayer(NULL, 0, 0));

    struct Player *player;
    for (int i = 0; i < 100; i++) {
        player = team_createPlayer("A", i, i);
        cut_assert_equal_string("A", player->name);
        cut_assert_equal_int(i, player->sockfd);
        cut_assert_equal_int(i, player->isHuman);
        cut_assert_equal_int(0, player->score);
        cut_assert_operator_int(-1, <, player->id);
        team_deletePlayer(&player);
    }
}

void test_team_createTeam()
{
    cut_assert_equal_pointer(NULL, team_createTeam(NULL));

    struct Team *team;
    for (int i = 0; i < 100; i++) {
        team = team_createTeam("A");
        cut_assert_equal_string("A", team->name);
        cut_assert_operator_int(-1, <, team->id);
        team_deleteTeam(&team);
    }
}

void test_team_addPlayer()
{
    cut_assert_not_equal_int(NO_ERROR, team_addPlayer(NULL, NULL));

    struct Team *team1 = team_createTeam("E");
    struct Player *testPlayer[MAX_TEAM_PLAYERS];
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        testPlayer[i] = team_createPlayer("A", i, i);
        cut_assert_equal_int(NO_ERROR, team_addPlayer(team1, testPlayer[i]));
        cut_assert_equal_int(DUPLICATE, team_addPlayer(team1, testPlayer[i]));
        int playerAdded = -1; //0 the player was added. Otherwise -1.
        for (int j = 0; j < MAX_TEAM_PLAYERS; j++)
            if (team1->players[j] == testPlayer[i])
                playerAdded = 0;
        cut_assert_equal_int(0, playerAdded);
    }

    struct Player *player1 = team_createPlayer("A", 1, 1);
    cut_assert_equal_int(TEAM_NULL, team_addPlayer(NULL, player1));
    cut_assert_equal_int(PLAYER_NULL, team_addPlayer(team1, NULL));
    cut_assert_equal_int(TEAM_FULL, team_addPlayer(team1, player1));

    team_deletePlayer(&player1);
    team_deleteTeam(&team1);

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        team_deletePlayer(&testPlayer[i]);
    }
}

void test_team_removePlayer()
{
    cut_assert_not_equal_int(NO_ERROR, team_removePlayer(NULL, NULL));

    struct Team *team1 = team_createTeam("A"); 
    struct Player *testPlayer[MAX_TEAM_PLAYERS];
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        testPlayer[i] = team_createPlayer("A", i, i);
        team_addPlayer(team1, testPlayer[i]);
    }

    cut_assert_equal_int(PLAYER_NULL, team_removePlayer(team1, NULL));
    cut_assert_equal_int(TEAM_NULL, team_removePlayer(NULL, testPlayer[0]));

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        cut_assert_equal_int(NO_ERROR, team_removePlayer(team1, testPlayer[i]));

        int removed = 1;
        for (int j = 0; j < MAX_TEAM_PLAYERS; j++) {
            if (team1->players[j] == testPlayer[i])
                removed = 0;
        }
        cut_assert_equal_int(1, removed);

        cut_assert_equal_int(NOT_FOUND,
                             team_removePlayer(team1, testPlayer[i]));
    }

    for (int i = 0; i < MAX_TEAM_PLAYERS; i++) {
        team_deletePlayer(&testPlayer[i]);
    }

    team_deleteTeam(&team1);
}

void test_team_deletePlayer()
{
    struct Player *player = team_createPlayer("A", 0, 0);

    cut_assert_equal_int(NO_ERROR, team_deletePlayer(&player));
    cut_assert_equal_pointer(NULL, player);
    cut_assert_equal_int(POINTER_NULL, team_deletePlayer(NULL));

}

void test_team_deleteTeam()
{
    struct Team *team = team_createTeam("A");

    cut_assert_equal_int(NO_ERROR, team_deleteTeam(&team));
    cut_assert_equal_pointer(NULL, team);
    cut_assert_equal_int(POINTER_NULL, team_deleteTeam(NULL));

}

void test_team_computeScore()
{
    struct Team *team = team_createTeam("A");
    struct Player *player1 = team_createPlayer("A", 0, 0);
    struct Player *player2 = team_createPlayer("B", 0, 0);

    team_addPlayer(team, player1);
    team_addPlayer(team, player2);

    player1->score = 3;
    player2->score = 5;
    
    cut_assert_equal_int(8, team_computeScore(team));

}

