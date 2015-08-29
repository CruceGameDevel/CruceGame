AM_CPPFLAGS = $(CUTTER_CFLAGS)  -I$(top_srcdir)/src/libCruceGame \
				-I$(top_srcdir)/src/libAsciiGui \
				-I$(top_srcdir)/src/networking \
				-DDEBUG

LDFLAGS = -module -rpath $(libdir) -avoid-version -no-undefined
CFLAGS = -std=c99
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"
echo-cutter:
	@echo $(CUTTER)

TESTS = run-test.sh

noinst_LTLIBRARIES = test_game.la test_networking.la
LIBS = $(CUTTER_LIBS) ${top_builddir}/src/libCruceGame.la \
                      ${top_builddir}/src/libNetworking.la

test_game_la_SOURCES = test-libCruceGame/test-deck.c \
		       test-libCruceGame/test-team.c \
		       test-libCruceGame/test-round.c \
		       test-libCruceGame/test-game.c

test_networking_la_SOURCES = test-networking/test-network.c

