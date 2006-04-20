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

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed the compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
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
])

# Checks for library functions.
AC_OUTPUT
