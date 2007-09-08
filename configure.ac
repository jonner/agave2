#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ(2.59)
AC_INIT([Agave],
        [0.9],
        [https://gna.org/bugs/?group=colorscheme],
        [agave])

AC_CONFIG_SRCDIR([src/color.cc])
AM_INIT_AUTOMAKE
GNOME_DOC_INIT

dnl Custom macro that checks if the git binary exists, and if it does, it tries
dnl to check the git repository version of the working copy and uses that in 
dnl the application's 'About' window
#GIT_REPOSITORY_REVISION()

PACKAGE_WEBSITE=[http://home.gna.org/colorscheme/]
AC_DEFINE_UNQUOTED([PACKAGE_WEBSITE], ["$PACKAGE_WEBSITE"],
                   [The website of Agave])
AC_SUBST([PACKAGE_WEBSITE])
PACKAGE_DESCRIPTION=["A Colorscheme Designer for the GNOME Desktop"]
AC_DEFINE_UNQUOTED([PACKAGE_DESCRIPTION], ["$PACKAGE_DESCRIPTION"],
                   [Description of Agave])
AC_SUBST([PACKAGE_DESCRIPTION])

AC_CONFIG_HEADER(config.h)

dnl Set Language to C++ and make sure proper tools exist
AC_LANG(C++)
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_CHECK_PROGS(SED, sed)

dnl i18n
AC_PROG_INTLTOOL([0.35.0])
GETTEXT_PACKAGE=agave
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain name to use with gettext])
AM_GLIB_GNU_GETTEXT

PKG_CHECK_MODULES(GTKMM, [
                          gtkmm-2.4 >= 2.10.0
                          ])
PKG_CHECK_MODULES(LIBGLADEMM, [
                               libglademm-2.4
                               ])

PKG_CHECK_MODULES([GNOME], [libgnomeui-2.0])

PKG_CHECK_MODULES([GCONFMM], [gconfmm-2.6])

dnl ***************
dnl gconf tool and path info
dnl ***************
AC_PATH_PROG(GCONFTOOL, gconftool-2)
dnl FIXME: this may break if gconf is not installed even if the user
dnl requests that gconf be disabled.  Do I need to copy this macro into my
dnl source tree??
AM_GCONF_SOURCE_2
AC_SUBST(GCONFTOOL)

dnl Determine whether to compile with debug settings.  Essentially this just
dnl disables compiler optimizations for now
AC_ARG_ENABLE(debug-mode, [AC_HELP_STRING([--enable-debug-mode],
              [build with debug settings (no optimization, test coverage)])],,
              [enable_debug_mode=no])
AC_MSG_CHECKING(whether to compile with debug settings)
if test  "x$AGAVE_DEVEL" = "xon" ; then
    enable_debug_mode=yes
    CXXFLAGS="$CXXFLAGS -Werror"
fi
if test "x$enable_debug_mode" = "xyes"; then
    AC_MSG_RESULT(yes)
    dnl remove optimization settings from CFLAGS
    CXXFLAGS=[`echo "$CXXFLAGS" | $SED -e 's/-O[0-9]*//g'`]
    CFLAGS=[`echo "$CFLAGS" | $SED -e 's/-O[0-9]*//g'`]
else
    AC_MSG_RESULT(no)
fi

dnl always compile with Wall
CXXFLAGS="$CXXFLAGS -Wall"

dnl Turn on the additional warnings last, so -Werror doesn't affect other tests.
AC_ARG_ENABLE(more-warnings,
[AC_HELP_STRING([--enable-more-warnings], [Maximum compiler warnings])],
set_more_warnings="$enableval",[set_more_warnings=no])
AC_MSG_CHECKING(for more warnings, including -Werror)
if test "$GXX" = "yes" -a "$set_more_warnings" != "no"; then
    AC_MSG_RESULT(yes)
    CXXFLAGS="-Wextra -Weffc++ -Wctor-dtor-privacy -Woverloaded-virtual -Wchar-subscripts -Wpointer-arith \
    -Wcast-align -Wsign-compare -Werror \
    $CXXFLAGS"

    for option in -Wno-strict-aliasing -Wno-sign-compare; do
        SAVE_CFLAGS="$CXXFLAGS"
        CFLAGS="$CXXFLAGS $option"
        AC_MSG_CHECKING([whether gcc understands $option])
        AC_TRY_COMPILE([], [],
                       has_option=yes,
                       has_option=no,)
        if test $has_option = no; then
            CXXFLAGS="$SAVE_CFLAGS"
        fi
        AC_MSG_RESULT($has_option)
        unset has_option
        unset SAVE_CFLAGS
    done
    unset option
else
    AC_MSG_RESULT(no)
fi

dnl test for boost::shared_ptr
AC_CHECK_HEADER([boost/shared_ptr.hpp], ,AC_MSG_ERROR(dnl
                [Boost header shared_ptr.hpp not found]))

dnl Consolidate all libraries and flags into one variable
AGAVE_LIBS="$GTKMM_LIBS $LIBGLADEMM_LIBS $GCONFMM_LIBS $GNOME_LIBS"
AGAVE_CFLAGS="$GTKMM_CFLAGS $LIBGLADEMM_CFLAGS $GCONFMM_CFLAGS $GNOME_CFLAGS"

AC_SUBST(AGAVE_LIBS)
AC_SUBST(AGAVE_CFLAGS)

dnl Define directory locations for use in the program
AGAVE_LOCALEDIR=[${datadir}/locale]
AGAVE_COMMONDIR=[${datadir}/agave]
AGAVE_ICONDIR=[${AGAVE_COMMONDIR}/pixmaps]
AGAVE_UIDIR=[${AGAVE_COMMONDIR}/ui]
AGAVE_PALETTEDIR=[${AGAVE_COMMONDIR}/palettes]
dnl pass the variables to automake
AC_SUBST([AGAVE_LOCALEDIR])
AC_SUBST([AGAVE_ICONDIR])
AC_SUBST([AGAVE_UIDIR])
AC_SUBST([AGAVE_PALETTEDIR])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/Doxyfile
                 data/Makefile
                 test/Makefile
                 po/Makefile.in
                 ])

AC_OUTPUT()

dnl ==================================================

echo ""
echo "======================================================================"
echo ""
echo "$PACKAGE_NAME configuration:"
echo "  C compiler              : $CC"
echo "  C++ compiler            : $CXX"
echo "  Prefix                  : $prefix"
echo ""
echo "The following options will be passed to the compiler:"
echo "  CFLAGS: $CFLAGS"
echo "  CXXFLAGS: $CXXFLAGS"
echo ""
echo "  AGAVE_CFLAGS: $AGAVE_CFLAGS"
echo ""
echo "  AGAVE_LIBS: $AGAVE_LIBS"
echo ""
echo "======================================================================"
echo ""
