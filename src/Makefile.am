AM_CPPFLAGS = -I$(top_srcdir)/src/libCruceGame
LDFLAGS = -no-undefined
CFLAGS = -std=c99 -Wall

lib_LTLIBRARIES = libCruceGame.la libAsciiGui.la
bin_PROGRAMS = cruceGame

cruceGame_SOURCES = libAsciiGui/main.c
cruceGame_LDFLAGS = -all-static
cruceGame_LDADD = libCruceGame.la libAsciiGui.la
cruceGame_LDFLAGS = -lcurses



libCruceGame_la_SOURCES = libCruceGame/deck.c \
			  libCruceGame/team.c \
			  libCruceGame/round.c
libAsciiGui_la_SOURCES =  libAsciiGui/output.c

