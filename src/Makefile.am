LDFLAGS = -no-undefined
CFLAGS = -std=c99 -Wall

lib_LTLIBRARIES = libdeck.la libteam.la libround.la

libdeck_la_SOURCES = deck.c
libteam_la_SOURCES = team.c
libround_la_SOURCES = round.c

# For every module you create, add libMODULE.la to lib_LTLIBRARIES (#3), where
# MODULE is the name of the module. Also, replace MODULE in the following line:
#
# libMODULE_la_SOURCES = MODULE.c
