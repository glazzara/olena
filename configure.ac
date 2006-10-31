#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])


## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Olena], [1.0], [olena@lrde.epita.fr], [olena])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
# 1.9.4 because of changelog vs. ChangeLog on Darwin.
AM_INIT_AUTOMAKE([1.9.4 foreign check-news dist-bzip2 -Wall])


## --------------------- ##
## C++ compiler set up.  ##
## --------------------- ##

# If the user doesn't provide any CXXFLAGS, prevent Autoconf from
# settings its own default ones (e.g., `-g -O2' for g++).
if test ! ${CXXFLAGS+set}; then
   CXXFLAGS=""
fi

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed the compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# C++ compiler flags for tests.
AC_ARG_VAR([TESTS_CXXFLAGS])
# We want no optimization for the tests (too slow), and a lot of debugging.
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS"; then
  TESTS_CXXFLAGS="-O0 -ggdb -Wall -pipe"
fi


## ------- ##
## Tests.  ##
## ------- ##

# Students should create their test suite in tests/.  Add the needed
# configure options here.
AC_CONFIG_FILES([
  metalic/tests/Makefile
  extended/tests/Makefile
  static/tests/Makefile
  olena/tests/Makefile
])


## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    metalic/Makefile
      metalic/mlc/Makefile
    extended/Makefile
      extended/xtd/Makefile
    static/Makefile
      static/stc/Makefile
    olena/Makefile
      olena/oln/Makefile
      olena/img/Makefile
    dynamic/Makefile
])
# FIXME: Should be performed through oln.m4 macros.
AC_CONFIG_FILES([
  olena/tests/algorithms/Makefile
  olena/tests/check/Makefile
  olena/tests/check/defs
  olena/tests/core/Makefile
  olena/tests/io/Makefile
  olena/tests/morphers/Makefile
  olena/tests/sanity/Makefile
  olena/tests/value/Makefile
])
AC_CONFIG_FILES([olena/tests/sanity/check],
		[chmod +x olena/tests/sanity/check])

AC_OUTPUT
