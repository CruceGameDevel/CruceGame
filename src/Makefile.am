AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame -I$(top_srcdir)/src/networking
LDFLAGS = -no-undefined
CFLAGS += -std=c99

if DEBUG
CFLAGS += -g -Wall -DDEBUG
endif

lib_LTLIBRARIES = libCruceGame.la libNetworking.la
bin_PROGRAMS = cruceGame

cruceGame_SOURCES = multiplayerCurses/main.c multiplayerCurses/cli.c
cruceGame_LDADD = libCruceGame.la libNetworking.la
cruceGame_LDFLAGS = -lncursesw -lpthread

libCruceGame_la_SOURCES = libCruceGame/deck.c \
			  libCruceGame/team.c \
			  libCruceGame/round.c \
                          libCruceGame/game.c

libNetworking_la_SOURCES = networking/network.c

