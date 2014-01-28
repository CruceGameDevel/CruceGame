AM_CPPFLAGS = $(CUTTER_CFLAGS) -I$(top_srcdir)/src/libCruceGame

LDFLAGS = -module -rpath $(libdir) -avoid-version -no-undefined
CFLAGS = -std=c99
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"
echo-cutter:
	@echo $(CUTTER)

TESTS = run-test.sh

noinst_LTLIBRARIES = test_game.la
LIBS = $(CUTTER_LIBS) ${top_builddir}/src/libCruceGame.la

test_game_la_SOURCES = test-deck.c test-team.c test-round.c

