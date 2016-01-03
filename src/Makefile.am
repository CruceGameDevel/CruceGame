AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame \
              -I$(top_srcdir)/src/networking   \
              -I$(top_srcdir)/src/parser \
              -I$(top_srcdir)/src/cli \
              -I$(top_srcdir)/src/irc \
              -I$(top_srcdir)/src
              
AM_LDFLAGS = -no-undefined
AM_CFLAGS  = -std=gnu99 \
             -DGAME_HELP_MANUAL="\"$(abs_top_builddir)/docs/man1/help.txt\""

if DEBUG
AM_CFLAGS += -g -Wall -DDEBUG
endif

lib_LTLIBRARIES = libCruceGame.la libNetworking.la libGameProtocol.la \
                  libCli.la libIrc.la
bin_PROGRAMS = cruceGame

libIrc_la_CPPFLAGS = $(AM_CPPFLAGS) -DIRC_SERVER=\"irc.freenode.net\"

cruceGame_SOURCES = singlePlayerCurses/singlePlayerCurses.c \
                    main.c

cruceGame_LDADD = libCruceGame.la libNetworking.la libGameProtocol.la \
                  libCli.la libIrc.la
cruceGame_LDFLAGS = -lncursesw -lpthread

libCruceGame_la_SOURCES    = libCruceGame/deck.c  \
                             libCruceGame/team.c  \
                             libCruceGame/round.c \
                             libCruceGame/game.c

libNetworking_la_SOURCES   = networking/network.c

libIrc_la_SOURCES          = irc/irc.c

libGameProtocol_la_SOURCES = gameProtocol/actionReceiver.c \
                             gameProtocol/actionSender.c

libCli_la_SOURCES = cli/cli.c


# Test Libs
lib_LTLIBRARIES += libCruceGameCheck.la libNetworkingCheck.la \
                   libNetworkingMock.la \
                   libGameProtocolCheck.la libCliCheck.la libIrcCheck.la

libIrcCheck_la_CPPFLAGS = $(AM_CPPFLAGS) -DIRC_SERVER=\"localhost\"

libCruceGameCheck_la_SOURCES    = libCruceGame/deck.c  \
                                  libCruceGame/team.c  \
                                  libCruceGame/round.c \
                                  libCruceGame/game.c

libNetworkingCheck_la_SOURCES   = networking/network.c

libNetworkingMock_la_SOURCES    = networking/network-mock.c

libIrcCheck_la_SOURCES          = irc/irc.c

libGameProtocolCheck_la_SOURCES = gameProtocol/actionReceiver.c \
                                  gameProtocol/actionSender.c

libCliCheck_la_SOURCES = cli/cli.c

