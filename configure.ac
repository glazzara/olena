#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])


## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([Olena], [1.0], [olena@lrde.epita.fr], [olena])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([build-aux/Makefile])

# Automake.
AM_INIT_AUTOMAKE([1.10 foreign check-news dist-bzip2 nostdinc -Wall])


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

# Speed up compiling times.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# ------------------------------ #
# C++ compiler flags for tests.  #
# ------------------------------ #

# FIXME: We might want to write an Autoconf macro to factor this.

# Standard flas for tests.
AC_ARG_VAR([TESTS_CXXFLAGS])
# We want no optimization for the tests (it slows down compiling
# times), and debugging information.
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS"; then
  TESTS_CXXFLAGS="-O0 -ggdb -Wall -W"
fi

# Flags for complex tests.
AC_ARG_VAR([TESTS_CXXFLAGS_SPEED])
# We want optimization for complex tests, and keep debugging flags
# (still useful).
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS_SPEED"; then
  TESTS_CXXFLAGS_SPEED="-O3 -DNDEBUG -ggdb -Wall -W"
fi

# Flags for tests with with all debugging features turned on.
AC_ARG_VAR([TESTS_CXXFLAGS_DEBUG])
# We want no optimization for the tests (it slows down compiling
# times), and a lot of debugging features.
# * GNU C++ Library Debug Mode:
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html
# * GNU C++ Library Compile Time Checks (a.k.a. concept checking):
#   http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt12ch29.html
if test "$GXX" = yes && test -z "$TESTS_CXXFLAGS_DEBUG"; then
  TESTS_CXXFLAGS_DEBUG="-O0 -ggdb -Wall -W -D_GLIBCXX_DEBUG -D_GLIBCXX_CONCEPT_CHECKS"
fi

# ----------------- #
# Flags for tools.  #
# ----------------- #

AC_ARG_VAR([TOOLS_CXXFLAGS])
# We want fast binaries for tools.
if test "$GXX" = yes && test -z "$TOOLS_CXXFLAGS"; then
  TOOLS_CXXFLAGS="-O3 -DNDEBUG -ggdb -Wall -W"
fi


## ----------- ##
## Libraries.  ##
## ----------- ##

# (Bundled) trimesh2 library.
AC_CONFIG_SUBDIRS([external/trimesh])

# FreeImage library.
OLN_WITH_FREEIMAGEPLUS
AM_CONDITIONAL([FREEIMAGEPLUS], [test x$oln_have_freeimageplus = xyes])

# Cfitsio library.
OLN_WITH_CFITSIO
AM_CONDITIONAL([CFITSIO], [test x$oln_have_cfitsio = xyes])


## --------- ##
## Swilena.  ##
## --------- ##

AC_PROG_LIBTOOL

OLN_WITH_SWILENA([with_swilena=yes], [with_swilena=no])
AM_CONDITIONAL([ENABLE_SWILENA], [test x$with_swilena = xyes])

AC_CONFIG_FILES([swilena/Makefile swilena/python/Makefile])
AC_CONFIG_FILES([swilena/run], [chmod +x swilena/run])


## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
    external/Makefile
    milena/Makefile
      milena/doc/Makefile
      milena/tools/Makefile
])


## ------- ##
## Tests.  ##
## ------- ##

# Ask for the creation of a milena/tests/data.hh, used to access to
# data (images) from tests.
AC_CONFIG_FILES([milena/tests/data.hh])

# Ask for the creation of tests' Makefiles.
# FIXME: Should it be achieved through oln.m4 macros, as we did in Olena 0.11?
AC_CONFIG_FILES([
  milena/tests/Makefile
    milena/tests/accu/Makefile
    milena/tests/algebra/Makefile
    milena/tests/arith/Makefile
    milena/tests/binarization/Makefile
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
      milena/tests/fun/vv2v/Makefile
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
    milena/tests/subsampling/Makefile
    milena/tests/tag/Makefile
    milena/tests/test/Makefile
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


## -------------- ##
## Applications.  ##
## -------------- ##

# Ask for the creation of applications' Makefiles.
# FIXME: Should it be achieved through oln.m4 macros, as we did in Olena 0.11?
AC_CONFIG_FILES([
  milena/apps/Makefile
    milena/apps/statues/Makefile
])

# Configure tests.
AC_CONFIG_FILES([milena/apps/statues/test],
		[chmod +x milena/apps/statues/test])


AC_OUTPUT
