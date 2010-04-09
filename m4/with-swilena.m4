#                                                       -*- Autoconf -*-

# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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


# FIXME: Revamp and make it generic, so that it can shared among
# Olena, TC and clspr.

# _OLN_WITH_SWIG(HAS-SWIG, HAS-NOT)
# ---------------------------------
# Check whether SWIG is requested, enabled, or disabled.  If
# requested, check that Python is present as needed.  If enabled,
# actually enable only if the environment is complete.
AC_DEFUN([_OLN_WITH_SWIG],
# FIXME: `with-swig' used to be set to `auto', but `no' is prefered
# now, as
# 1. users don't necessarily want to build Swilena by default;
# 2. the `auto' flag is not honored by the checks for Python and Ruby.
[AC_ARG_WITH([swig],
	    [AC_HELP_STRING([--with-swig],
			    [require SWIG modules (defaults to no)])],
            [],
	    [with_swig=no])

case $with_swig:$enable_shared in
  auto:no)
  AC_MSG_NOTICE([SWIG use disabled: dynamic libraries disabled])
  with_swig=no
  ;;
esac

if test x$with_swig != xno; then
  oln_has_swig=yes
  AM_PATH_PYTHON([2.4])
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

# FIXME: Disabled.  Swilena does not support Ruby again (as it used to
# in Olena 0.11).  Moreover, the user should be given the choice
# between the front ends (Python, Ruby, etc.); enabling Swilena shall
# not systematically imply compiling the *whole* back ends!
#
#   # Check for Ruby.
#   AC_CHECK_PROGS([RUBY], [ruby], [$am_aux_dir/missing ruby])
#   if test "x$RUBY" = "x$am_aux_dir/missing ruby"; then
#     AC_MSG_ERROR([no suitable Ruby interpreter found])
#   fi
# 
#   AC_ARG_VAR([RUBY_CPPFLAGS], [Path to ruby.h])
#   AC_ARG_WITH([ruby-includedir], [Include path for ruby.h],
# 	      [RUBY_CPPFLAGS="-I$withval"],
# 	      [RUBY_CPPFLAGS="-I`$RUBY -e 'puts $:.join("\n")' | \
# 		 while read line
# 		   do
# 		   if [[ -f ${line}/ruby.h ]]; then
# 		     echo $line
# 		   fi
# 		 done`"])
#   save_CPPFLAGS=$CPPFLAGS
#   CPPFLAGS="$CPPFLAGS $RUBY_CPPFLAGS"
#   AC_CHECK_HEADERS([ruby.h], [],
#     [AC_MSG_ERROR(
# 	    [You need Ruby development files to compile the Ruby interface.])])
#   CPPFLAGS=$save_CPPFLAGS

  AC_PROG_SWIG([1.3.35])
  if (eval "$SWIG -version") >/dev/null 2>&1; then :; else
    oln_has_swig=no
  fi

  case $with_swig:$oln_has_swig in
    yes:no)
      AC_MSG_ERROR([SWIG 1.3.35 is required.
		    Use `--without-swig' to disable SWIG modules.]);;
  esac
fi

case $oln_has_swig in
  yes) $1;;
  * )  $2;;
esac
])

# OLN_WITH_SWILENA(WITH, WITHOUT)
# -------------------------------
# Should we build Swilena or not.
AC_DEFUN([OLN_WITH_SWILENA],
[AC_CACHE_CHECK([whether building Swilena],
                [oln_cv_with_swilena],
                [_OLN_WITH_SWIG([oln_cv_with_swilena=yes],
                                [oln_cv_with_swilena=no])])
case $oln_cv_with_swilena in
  yes) $1;;
  no ) $2;;
  *)   AC_MSG_ERROR([incorrect oln_cv_with_swilena value: $oln_cv_with_swilena]);;
esac
])
