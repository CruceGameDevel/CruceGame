AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame
LDFLAGS = -no-undefined
CFLAGS += -std=c99

if DEBUG
CFLAGS += -g -Wall -DDEBUG
endif

lib_LTLIBRARIES = libCruceGame.la libParser.la
bin_PROGRAMS = cruceGame

cruceGame_SOURCES = cruceGameCurses/main.c cruceGameCurses/cli.c
cruceGame_LDADD = libCruceGame.la libParser.la
cruceGame_LDFLAGS = -lncursesw

libCruceGame_la_SOURCES = libCruceGame/deck.c \
			  libCruceGame/team.c \
			  libCruceGame/round.c \
                          libCruceGame/game.c
libParser_la_SOURCES    = parser/parser.c
