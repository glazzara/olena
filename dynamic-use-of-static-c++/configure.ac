# Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.61])

# Catch some macros that are not expanded.
m4_pattern_forbid([^AC_LIBLTDL_])
m4_pattern_forbid([^DYN_])
m4_pattern_allow([^DYN_FIXTURES$])
m4_pattern_forbid([^ERB$])
m4_pattern_forbid([^RBCONFIG$])

## ---------------- ##
## Package set up.  ##
## ---------------- ##

AC_INIT([LRDE C++ Static-Dynamic bridge], [0.1],
        [nicolas.pouillard@lrde.epita.fr, damien.thivolle@lrde.epita.fr], [dyn])

# Auxiliary files.
AC_CONFIG_AUX_DIR([_config])

# Automake.
AM_INIT_AUTOMAKE([1.10 foreign check-news dist-bzip2 -Wall nostdinc])


## --------------------- ##
## C++ Compiler Set up.  ##
## --------------------- ##

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

# Speed the compilation up.
if test "$GXX" = yes; then
  CXXFLAGS="$CXXFLAGS -pipe"
fi

# Use good warnings.
DYN_CXX_WARNINGS([[-Wall],
                  [-W],
                  [-Wcast-align],
                  [-Wcast-qual],
                  [-Wformat],
                  [-Wmissing-prototypes],
                  [-Wstrict-prototypes],
                  [-Wwrite-strings],
                  [-Wbad-function-cast],
                  [-Wmissing-declarations],
                  [-Wnested-externs],
                  [-Werror]])

## -------- ##
## Libtool. ##
## -------- ##

# Configure libltdl
AC_CONFIG_SUBDIRS(libltdl)

AC_DISABLE_STATIC
# Use the installable version of ltdl

# AC_LIBLTDL_INSTALLABLE
# INSTALL_LTDL

AC_LIBLTDL_CONVENIENCE

# Check for dlopen support
AC_LIBTOOL_DLOPEN
# We don't need static libraries, speed the compilation up.
AC_DISABLE_STATIC
# Configure libtool
AC_PROG_LIBTOOL

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

## ------ ##
## Boost. ##
## ------ ##

BOOST_FILESYSTEM
BOOST_STRING_ALGO

dnl  ## ------ ##
dnl  ## Swig.  ##
dnl  ## ------ ##

dnl  AC_ARG_WITH([swig],
            dnl  [AC_HELP_STRING([--with-swig],
                            dnl  [require Swig modules (defaults to auto)])],
            dnl  [],
            dnl  [with_swig=auto])

dnl  if test x$with_swig != xno; then
  dnl  has_swig=yes

  dnl  # Check for python and swig

  dnl  AC_PROG_SWIG([1.3.24])
  dnl  if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
    dnl  has_swig=no
  dnl  fi

  dnl  case $with_swig:$has_swig in
    dnl  yes:no)
      dnl  AC_MSG_ERROR([SWIG 1.3.24 is required.
                    dnl  Use `--without-swig' to disable SWIG modules.]);;
  dnl  esac
dnl  fi

dnl  AC_MSG_RESULT([$has_swig])

# FIXME: Doesn't Autoconf provide something like abs_top_srcdir?
my_abs_srcdir=`cd $srcdir && pwd`
# FIXME: Likewise.
my_abs_builddir=`pwd`

# FIXME: Remove me when function_loader will be pure C++
AC_SUBST([DYNDIR], [$my_abs_srcdir/src])

AC_SUBST([DYN_DATADIR], [$my_abs_builddir/data])

AC_SUBST([DYN_FIXTURES], [$my_abs_srcdir/test/fixtures])

# Absolute path to Milena.
AC_SUBST([MILENA_DIR], [$my_abs_srcdir/../milena])

AC_SUBST([LIBERTYARG],
         $my_abs_srcdir/$(ERB([lib/libiberty-<%= RUBY_PLATFORM %>.la])))


## ------------------- ##
## Ruby Configuration. ##
## ------------------- ##

# Make available Config::CONFIG
# AC_DEFINE and AC_SUBST everything in this hash
# All keys are prefixed by RUBY_.
RBCONFIG()

# ...
echo $srcdir/config/local-config.rb.in
MK_LOCAL_CONFIG_RB_IN([$srcdir/_config/local-config.rb.in])

## --------------- ##
## Configuration.  ##
## --------------- ##

# Ask for config.h.
AM_CONFIG_HEADER([_config/config.h])

# Ask for the Makefile creations.
AC_CONFIG_FILES([
  Makefile
  bin/Makefile
  bin/dyn-config
  libmd5/Makefile
  src/Makefile
  src/config.hh
  data/Makefile
  config/Makefile
  _config/local-config.rb
  test/Makefile
    test/vaucanson/Makefile
], [chmod +x bin/dyn-config])

# Instantiate the output files.
AC_OUTPUT
