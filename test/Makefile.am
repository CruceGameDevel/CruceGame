INCLUDES = $(CUTTER_CFLAGS) -I$(top_srcdir)/src

LDFLAGS = -module -rpath $(libdir) -avoid-version -no-undefined
CFLAGS = -std=c99
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"
echo-cutter:
	@echo $(CUTTER)

TESTS = run-test.sh

noinst_LTLIBRARIES = test_deck.la test_team.la test_round.la
LIBS = $(CUTTER_LIBS) ${top_builddir}/src/libdeck.la \
                      ${top_builddir}/src/libteam.la \
                      ${top_builddir}/src/libround.la

test_deck_la_SOURCES = test-deck.c
test_team_la_SOURCES = test-team.c
test_round_la_SOURCES = test-round.c


# For every module you create:
# - create a file test-MODULE.c, where you will put tests
# - add test_MODULE.la to noinst_LTLIBRARIES (#10)
# - add ${top_builddir)/src/../libMODULE.la to LIBS (#11)
# - add the following line:
#
# test_stack_la_SOURCES = test-MODULE.c
