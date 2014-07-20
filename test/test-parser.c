#include "src/parser/parser.h"
#include <libCruceGame.h>

void test_parser_parse_parameterCheck()
{
    char string[] = "string";
    struct Parser *parser = malloc(sizeof(struct Parser));
    cut_assert_equal_int(POINTER_NULL, parser(NULL, NULL));
    cut_assert_equal_int(POINTER_NULL, parser(string, NULL));
    cut_assert_equal_int(POINTER_NULL, parser(NULL, parser));
    cut_assert_not_equal_int(POINTER_NULL, parser(string, parser));
}

void test_parser_parse_chatLine()
{
    char string[] = "This is a chat line";
    struct Parser *parser = malloc(sizeof(struct Parser));
    cut_assert_equal_int(CHAT_LINE, parse(string, parser));
}

void test_parser_onCreateGame()
{
    struct Parser *parser = createParser();
    char *line1[] = "CREATE_GAME: 11";
    char *line2[] = "CREATE_GAME: 22";
    char *line3[] = "CREATE_GAME: 31";
    cut_assert_not_equal_int(0,  parse(line1, parser));
    cut_assert_equal_int(11, parser->game->pointsNumber);
    cut_assert_not_equal_int(0,  parse(line2, parser));
    cut_assert_equal_int(22, parser->game->pointsNumber);
    cut_assert_equal_pointer(NULL, parser(line2, parser));
    cut_assert_equal_pointer(NULL, parser->game);
    deleteParser(&parser);
}

