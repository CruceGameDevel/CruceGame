AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame \
              -I$(top_srcdir)/src/networking   \
              -I$(top_srcdir)/src/parser \
              -I$(top_srcdir)/src/cli \
              -I$(top_srcdir)/src/irc \
              -I$(top_srcdir)/src
              
LDFLAGS = -no-undefined
CFLAGS += -std=c99 \
          -DGAME_HELP_MANUAL="\"$(abs_top_builddir)/docs/man1/help.txt\""

if DEBUG
CFLAGS += -g -Wall -DDEBUG
endif

lib_LTLIBRARIES = libCruceGame.la libNetworking.la libGameProtocol.la libCli.la
lib_LTLIBRARIES += libIrc.la
bin_PROGRAMS = cruceGame

cruceGame_SOURCES = singlePlayerCurses/singlePlayerCurses.c \
                    main.c

cruceGame_LDADD = libCruceGame.la libNetworking.la libGameProtocol.la libCli.la
cruceGame_LDADD += libIrc.la
cruceGame_LDFLAGS = -lncursesw -lpthread

libCruceGame_la_SOURCES = libCruceGame/deck.c \
                          libCruceGame/team.c \
                          libCruceGame/round.c \
                          libCruceGame/game.c

libNetworking_la_SOURCES = networking/network.c

libIrc_la_SOURCES = irc/irc.c

libGameProtocol_la_SOURCES = gameProtocol/actionReceiver.c \
                             gameProtocol/actionSender.c

libCli_la_SOURCES = cli/cli.c
