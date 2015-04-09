AC_INIT([Fringe Search], [0.3], [fongog@gmail.com], [pastar])


AC_PREREQ([2.62])
AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(foreign)
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

NAME_VERSION=$PACKAGE-$VERSION

AC_DEFINE_UNQUOTED([PROGRAM_NAME], "$PACKAGE", [Parallel Astar])

AC_SUBST(NAME_VERSION)

AC_GNU_SOURCE

LT_INIT

# ----------------------------------------------------------------------
# Checks for programs.
# ----------------------------------------------------------------------

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

# -----------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------


CFLAGS="$CFLAGS -ggdb -lm -pg"

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
