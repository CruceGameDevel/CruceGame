AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame
LDFLAGS = -no-undefined
CFLAGS = -std=c99 -Wall

lib_LTLIBRARIES = libCruceGame.la libAsciiGui.la

libCruceGame_la_SOURCES = libCruceGame/deck.c \
			  libCruceGame/team.c \
			  libCruceGame/round.c
libAsciiGui_la_SOURCES =  libAsciiGui/output.c
