AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame \
	      -I$(top_srcdir)/src/networking   \
	      -I$(top_srcdir)/src/parser
LDFLAGS = -no-undefined
CFLAGS += -std=c99

if DEBUG
CFLAGS += -g -Wall -DDEBUG
endif

lib_LTLIBRARIES = libCruceGame.la libNetworking.la libParser.la
bin_PROGRAMS = cruceGame

cruceGame_SOURCES = multiplayerCurses/multiplayerCurses.c \
		    multiplayerCurses/ircHandlers.c \
		    singlePlayerCurses/singlePlayerCurses.c \
		    cli/cli.c \
		    main.c

cruceGame_LDADD = libCruceGame.la libNetworking.la libParser.la
cruceGame_LDFLAGS = -lncursesw -lpthread

libCruceGame_la_SOURCES = libCruceGame/deck.c \
			  libCruceGame/team.c \
			  libCruceGame/round.c \
                          libCruceGame/game.c

libNetworking_la_SOURCES = networking/network.c

libParser_la_SOURCES = parser/parser.c
