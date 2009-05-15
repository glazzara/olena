#                                                       -*- Autoconf -*-

# OLN_WITH_CFITSIO
# ----------------------
# Checks whether the CFITSIO library is available from C++
# programs.
#
# This macro sets CFITSIO_CXXFLAGS and CFITSIO_LDFLAGS if
# the library is found and its functions available from C++.
AC_DEFUN([OLN_WITH_CFITSIO],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 AC_ARG_WITH([cfitsio],
   [AC_HELP_STRING([--with-cfitsio@<:@=DIR@:>@],
     [using CFITSIO (DIR = prefix for CFITSIO installation)])])
 CFITSIO_CXXFLAGS=''
 CFITSIO_LDFLAGS=''
 if test "x$with_cfitsio" != xno; then
   if test -n "$with_cfitsio"; then
     CFITSIO_CXXFLAGS="-I${with_cfitsio}/include"
     CFITSIO_LDFLAGS="-L${with_cfitsio}/lib"
   fi
   oln_save_CXXFLAGS=$CXXFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CXXFLAGS="$CXXFLAGS $CFITSIO_CXXFLAGS"
   LDFLAGS="$LDFLAGS $CFITSIO_LDFLAGS"
   oln_have_cfitsio=no
   AC_CHECK_HEADER([fitsio.h],
     [AC_CHECK_LIB([cfitsio],
       [main],
       [oln_have_cfitsio=yes
         CFITSIO_LDFLAGS="$CFITSIO_LDFLAGS -lcfitsio"
         AC_DEFINE([HAVE_CFITSIO], 1,
                   [Define to 1 if we can use CFITSIO])
       ]
     )]
   )
   CXXFLAGS=$oln_save_CXXFLAGS
   LDFLAGS=$oln_save_LDFLAGS
   TOOLS_LDFLAGS="$TOOLS_LDFLAGS $CFITSIO_LDFLAGS"
 fi
 AC_SUBST([CFITSIO_CXXFLAGS])
 AC_SUBST([CFITSIO_LDFLAGS])

 AC_LANG_POP([C++])
])
