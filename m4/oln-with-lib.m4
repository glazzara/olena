#                                                       -*- Autoconf -*-

# OLN_WITH_LIB(PACKAGE, HEADER, LIBRARY, [SHELL_NAME], [CPP_NAME])
# ----------------------------------------------------------------
# Checks whether library PACKAGE is available from C++ programs.
# HEADER and LIBRARY are respectively a header and the library name
# (object) that are checked to assess the usability of the library.
# SHELL_NAME is a name used in shell variables (preferably in lower
# case), while CPP_NAME is used in the name of C++ preprocessor macros
# and flags (preferably in upper case).
#
# If they are not given, SHELL_NAME defaults to m4_tolower([LIBRARY])
# and CPP_NAME defaults to m4_toupper([LIBRARY]).
#
# This macro sets the shell variable oln_have_`SHELL_NAME' to `yes' if
# the library is found and if it is working (and to `no' otherwise),
# and defines a CPP macro and an Automake conditional both named
# HAVE_`CPP_NAME'.  It also sets `CPP_NAME'_CXXFLAGS and
# `CPP_NAME'_LDFLAGS.
#
# Examples of use:
#
#   OLN_WITH_LIB([FreeImagePlus], [FreeImagePlus.h], [freeimageplus])
#   OLN_WITH_LIB([CFITSIO], [fitsio.h], [cfitsio])
#   OLN_WITH_LIB([Zlib], [zlib.h], [z], [zlib], [ZLIB])
#
AC_DEFUN([OLN_WITH_LIB],
[dnl Do some sanity checking of the arguments.
m4_if([$1], , [AC_FATAL([$0: missing argument 1])])dnl
m4_if([$2], , [AC_FATAL([$0: missing argument 2])])dnl
m4_if([$3], , [AC_FATAL([$0: missing argument 3])])dnl
_OLN_WITH_LIB([$1], [$2], [$3],dnl
dnl Process optional arguments.
m4_default([$4], m4_tolower([$3])),dnl
m4_default([$5], m4_toupper([$3])))dnl
])# OLN_WITH_LIB


# _OLN_WITH_LIB(PACKAGE, HEADER, LIBRARY, SHELL_NAME, CPP_NAME)
# -------------------------------------------------------------
# Actual implementation of OLN_WITH_LIB.
AC_DEFUN([_OLN_WITH_LIB],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])
 AC_ARG_WITH([$4],
   [AC_HELP_STRING([--with-$4@<:@=DIR@:>@],
     [use $1 (DIR = prefix for $1 installation)])])
 $5_CXXFLAGS=''
 $5_LDFLAGS=''
 if test "x$with_$4" != xno; then
   if test -n "$with_$4"; then
     $5_CXXFLAGS="-I${with_$4}/include"
     $5_LDFLAGS="-L${with_$4}/lib"
   fi
   oln_save_CXXFLAGS=$CXXFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CXXFLAGS="$CXXFLAGS $$5_CXXFLAGS"
   LDFLAGS="$LDFLAGS $$5_LDFLAGS"
   oln_have_$4=no
   AC_CHECK_HEADER([$2],
     [AC_CHECK_LIB([$3],
       [main],
       [oln_have_$4=yes
         $5_LDFLAGS="$$5_LDFLAGS -l$3"
         AC_DEFINE([HAVE_$5], 1, [Define to 1 if we can use $1])
       ]
     )]
   )
   CXXFLAGS=$oln_save_CXXFLAGS
   LDFLAGS=$oln_save_LDFLAGS
   TOOLS_LDFLAGS="$TOOLS_LDFLAGS $$5_LDFLAGS"
 fi
 AC_SUBST([$5_CXXFLAGS])
 AC_SUBST([$5_LDFLAGS])
 AM_CONDITIONAL([HAVE_$5], [test x$oln_have_$4 = xyes])
 AC_LANG_POP([C++])
])# _OLN_WITH_LIB
