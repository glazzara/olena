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

## ----------- ##
## Libiberty.  ##
## ----------- ##

DYN_LIBIBERTY

# Are we running on Darwin (Mac OS X)?  If so, we'll have to deal with
# MacPorts' libiberty's issues w.r.t `environ'.
AM_CONDITIONAL([DARWIN], [echo "$host_os" | grep '^darwin'])

## ------ ##
## Boost. ##
## ------ ##

BOOST_FILESYSTEM
BOOST_STRING_ALGO

## ------ ##
## Swig.  ##
## ------ ##

# FIXME: Unify the Autoconf machinery w.r.t. SWIG and Python with Olena's.

AC_ARG_WITH([swig],
            [AC_HELP_STRING([--with-swig],
                            [require Swig modules (defaults to auto)])],
            [],
            [with_swig=auto])

if test x$with_swig != xno; then
  has_swig=yes

  # Check for Python.
  AM_PATH_PYTHON([2.5])
  adl_CHECK_PYTHON

  # Check for Python and SWIG.
  save_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS -I$PYTHONINC"
  AC_CHECK_HEADERS([Python.h],
		   [python_headers=yes],
	           [python_headers=no])

  if test x$python_headers = xno; then
    oln_has_swig=no
    if test x$with_swig = xyes; then
      AC_MSG_ERROR(
	  [Python.h is required to build SWIG modules.
	  Add `-I python_include_path' to `CPPFLAGS'
	  or `--without-swig' to disable SWIG modules.])
    fi
  fi

  CPPFLAGS=$save_CPPFLAGS

  AC_PROG_SWIG([1.3.36])
  if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
    has_swig=no
  fi

  case $with_swig:$has_swig in
    yes:no)
      AC_MSG_ERROR([SWIG 1.3.36 is required.
                    Use `--without-swig' to disable SWIG modules.]);;
  esac
fi

AC_MSG_RESULT([$has_swig])

AC_CONFIG_FILES([swig/Makefile
		 swig/python/Makefile])

AC_CONFIG_FILES([swig/run], [chmod +x swig/run])



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
    src/wrappers/Makefile
  data/Makefile
  config/Makefile
  _config/local-config.rb
  test/Makefile
    test/vaucanson/Makefile
    test/wrappers/Makefile
], [chmod +x bin/dyn-config])

# Instantiate the output files.
AC_OUTPUT
