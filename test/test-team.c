#include <team.h>
#include <cutter.h>

static struct Team team;

void test_team_createPlayer()
{
    int check[100];
    for (int i = 0; i < 100; i++) 
        check[i] = 0;
    struct Player *player;
    for (int i = 0; i < 100; i++) {
        player = createPlayer("A", i, i);
        cut_assert_equal_string("A", player->name);
        cut_assert_equal_int(i, player->sockfd);
        cut_assert_equal_int(i, player->isHuman);
        cut_assert_equal_int(0, player->score);
        cut_assert_operator_int(-1, <, player->id);
        cut_assert_operator_int(101, >, player->id);
        check[player->id] = 1;
    }
    for (int i = 0; i < 100; i++)
        cut_assert_not_equal_int(check[i], 0);
}

void test_team_createTeam()
{
    int check[100];
    for (int i = 0; i < 100; i++) 
        check[i] = 0;
    struct Team *team;
    for (int i = 0; i < 100; i++) {
        team = createTeam("A");
        cut_assert_equal_string("A", team->name);
        cut_assert_operator_int(-1, <, team->id);
        cut_assert_operator_int(101, >, team->id);
        check[team->id] = 1;
    }
    for (int i = 0; i < 100; i++)
        cut_assert_not_equal_int(check[i], 0);
}

