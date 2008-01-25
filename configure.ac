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


## -------------- ##
## Dependencies.  ##
## -------------- ##

# FreeImage library.
OLN_WITH_FREEIMAGEPLUS
AM_CONDITIONAL([FREEIMAGEPLUS], [test x$oln_have_freeimageplus = xyes])

# Cfitsio library.
OLN_WITH_CFITSIO
AM_CONDITIONAL([CFITSIO], [test x$oln_have_cfitsio = xyes])


## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    milena/Makefile
      milena/doc/Makefile
])


## ------- ##
## Tests.  ##
## ------- ##

# Ask for the creation of a milena/tests/data.hh, used to access to
# data (images) from tests.
AC_CONFIG_FILES([milena/tests/data.hh])

# FIXME: Should be performed through oln.m4 macros, as we did in Olena 0.11?
# Ask for the creation of tests' Makefiles.
AC_CONFIG_FILES([
  milena/tests/Makefile
    milena/tests/accu/Makefile
    milena/tests/arith/Makefile
    milena/tests/border/Makefile
    milena/tests/canvas/Makefile
      milena/tests/canvas/browsing/Makefile
      milena/tests/canvas/morpho/Makefile
    milena/tests/convert/Makefile
    milena/tests/core/Makefile
    milena/tests/debug/Makefile
    milena/tests/display/Makefile
    milena/tests/draw/Makefile
    milena/tests/estim/Makefile
    milena/tests/fun/Makefile
      milena/tests/fun/i2v/Makefile
      milena/tests/fun/p2b/Makefile
      milena/tests/fun/p2v/Makefile
      milena/tests/fun/v2v/Makefile
      milena/tests/fun/x2x/Makefile
    milena/tests/geom/Makefile
    milena/tests/histo/Makefile
    milena/tests/io/Makefile
      milena/tests/io/fits/Makefile
      milena/tests/io/pbm/Makefile
      milena/tests/io/pfm/Makefile
      milena/tests/io/pgm/Makefile
      milena/tests/io/pnm/Makefile
      milena/tests/io/ppm/Makefile
    milena/tests/labeling/Makefile
    milena/tests/level/Makefile
      milena/tests/level/approx/Makefile
      milena/tests/level/naive/Makefile
    milena/tests/linear/Makefile
      milena/tests/linear/local/Makefile
    milena/tests/literal/Makefile
    milena/tests/logical/Makefile
    milena/tests/math/Makefile
    milena/tests/metal/Makefile
      milena/tests/metal/make/Makefile
      milena/tests/metal/math/Makefile
    milena/tests/morpho/Makefile
    milena/tests/neighb/Makefile
    milena/tests/norm/Makefile
    milena/tests/pw/Makefile
    milena/tests/set/Makefile
    milena/tests/tag/Makefile
    milena/tests/trace/Makefile
    milena/tests/trait/Makefile
      milena/tests/trait/image/Makefile
      milena/tests/trait/op/Makefile
      milena/tests/trait/value/Makefile
    milena/tests/util/Makefile
    milena/tests/value/Makefile
      milena/tests/value/builtin/Makefile
      milena/tests/value/concept/Makefile
    milena/tests/win/Makefile
])

AC_OUTPUT
