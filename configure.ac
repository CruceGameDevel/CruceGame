AC_PREREQ(2.59)

AC_INIT([cruceGame], [0.2.0])

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_LIBTOOL

AC_CHECK_HEADERS([curses.h])

AC_CHECK_CUTTER
AM_CONDITIONAL(CUTTER, test x"$cutter_use_cutter" = x"yes")

AC_ARG_ENABLE(debug,
              [--enable-debug   Enable debug flags],
              [case "$enableval" in
                  yes) debug=true  ;;
                  no)  debug=false ;;
                  *)   AC_MSG_ERROR(Bad value ${enableval} for --enable-debug);;
               esac],
              [debug=false])
if test "$debug" = "true"; then
    echo "DEBUG MODE ENABLED"
else
    echo "DEBUG MODE DISABLED"
fi
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])

AC_OUTPUT
