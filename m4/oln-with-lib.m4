#                                                       -*- Autoconf -*-

# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
# (LRDE)
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


# OLN_WITH_LIB(PACKAGE, HEADER, LIBRARY, [SHELL_NAME], [CPP_NAME],
#              [OTHER-LIBRARIES])
# ----------------------------------------------------------------
# Checks whether library PACKAGE is available from C++ programs.
# HEADER and LIBRARY are respectively a header and the library name
# (object) that are checked to assess the usability of the library.
# SHELL_NAME is a name used in shell variables (preferably in lower
# case), while CPP_NAME is used in the name of C++ preprocessor macros
# and flags (preferably in upper case).  OTHER-LIBRARIES are
# additional libraries that LIBRARY depends upon, that should be
# passed as a space-separated list of linker arguments (e.g.
# `-lXt -lX11').
#
# If they are not given, SHELL_NAME defaults to m4_tolower([LIBRARY])
# and CPP_NAME defaults to m4_toupper([LIBRARY]).
#
# This macro sets the shell variable oln_have_`SHELL_NAME' to `yes' if
# the library is found and if it is working (and to `no' otherwise),
# and defines a CPP macro and an Automake conditional both named
# HAVE_`CPP_NAME'.  It also sets `CPP_NAME'_CPPFLAGS and
# `CPP_NAME'_LDFLAGS.
#
# Examples of use:
#
#   OLN_WITH_LIB([FreeImagePlus], [FreeImagePlus.h], [freeimageplus])
#   OLN_WITH_LIB([CFITSIO], [fitsio.h], [cfitsio])
#   OLN_WITH_LIB([Zlib], [zlib.h], [z], [zlib], [ZLIB])
#   OLN_WITH_LIB([GDCM], [gdcm-2.0/gdcmReader.h], [gdcmCommon], [gdcm], [GDCM],
#                [-lgdcmzlib])
#
AC_DEFUN([OLN_WITH_LIB],
[dnl Do some sanity checking of the arguments.
m4_if([$1], , [AC_FATAL([$0: missing argument 1])])dnl
m4_if([$2], , [AC_FATAL([$0: missing argument 2])])dnl
m4_if([$3], , [AC_FATAL([$0: missing argument 3])])dnl
_OLN_WITH_LIB([$1], [$2], [$3],dnl
dnl Process optional arguments.
m4_default([$4], m4_tolower([$3])),dnl
m4_default([$5], m4_toupper([$3])),dnl
[$6])dnl
])# OLN_WITH_LIB


# _OLN_WITH_LIB(PACKAGE, HEADER, LIBRARY, SHELL_NAME, CPP_NAME,
#               [OTHER-LIBRARIES])
# -------------------------------------------------------------
# Actual implementation of OLN_WITH_LIB.
AC_DEFUN([_OLN_WITH_LIB],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])
 AC_ARG_WITH([$4],
   [AC_HELP_STRING([--with-$4@<:@=DIR@:>@],
     [use $1 (DIR = prefix for $1 installation)])])
 $5_CPPFLAGS=''
 $5_LDFLAGS=''
 base_inc_dir=''
 oln_have_$4=no
 if test "x$with_$4" != xno; then
   if test -n "$with_$4"; then
     base_inc_dir="${with_$4}/include/"
     $5_CPPFLAGS="-I${base_inc_dir}"
     $5_LDFLAGS="-L${with_$4}/lib"
   fi
   oln_save_CPPFLAGS=$CPPFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CPPFLAGS="$CPPFLAGS $$5_CPPFLAGS"
   LDFLAGS="$LDFLAGS $$5_LDFLAGS"
   header="${base_inc_dir}$2"
   AC_CHECK_HEADER([$header],
     [AC_CHECK_LIB([$3],
       [main],
       [oln_have_$4=yes
         $5_LDFLAGS="$$5_LDFLAGS -l$3 $6"
         AC_DEFINE([HAVE_$5], 1, [Define to 1 if we can use $1])],
       [], [$6]
     )]
   )
   CPPFLAGS=$oln_save_CPPFLAGS
   LDFLAGS=$oln_save_LDFLAGS
   TOOLS_LDFLAGS="$TOOLS_LDFLAGS $$5_LDFLAGS"
 fi
 AC_SUBST([$5_CPPFLAGS])
 AC_SUBST([$5_LDFLAGS])
 AM_CONDITIONAL([HAVE_$5], [test x$oln_have_$4 = xyes])
 AC_LANG_POP([C++])
])# _OLN_WITH_LIB
