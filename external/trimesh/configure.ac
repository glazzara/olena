AC_PREREQ([2.61])

AC_INIT([trimesh2], [2.8], [smr at princeton edu])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])

# Automake.
AM_INIT_AUTOMAKE([1.10 foreign dist-bzip2 nostdinc -Wall])

## --------------------- ##
## C++ compiler set up.  ##
## --------------------- ##

# If the user doesn't provide any CXXFLAGS, prevent Autoconf from
# settings its own default ones (e.g., `-g -O2' for g++).
if test ! ${CXXFLAGS+set}; then
   CXXFLAGS=""
fi

# Look for a C compiler (used by freeglut).
AC_LANG([C])
AC_PROG_CC

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Look for ranlib.
AC_PROG_RANLIB

# Speed up compiling times.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

## ------------------ ##
## Libraries set up.  ##
## ------------------ ##

# Use Libtool.
AC_PROG_LIBTOOL

# Check for Darwin.
AC_CANONICAL_HOST
AM_CONDITIONAL([DARWIN], [echo "$host_os" | grep '^darwin'])

## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([Makefile
                 gluit/Makefile
                 include/Makefile
                 libsrc/Makefile
                 utilsrc/Makefile])

AC_OUTPUT
