#                                                       -*- Autoconf -*-

# OLN_WITH_FREEIMAGEPLUS
# ----------------------
# Checks whether the FreeImagePlus library is available from C++
# programs.
#
# This macro sets FREEIMAGEPLUS_CXXFLAGS and FREEIMAGEPLUS_LDFLAGS if
# the library is found and its functions available from C++.
AC_DEFUN([OLN_WITH_FREEIMAGEPLUS],
[dnl
 AC_REQUIRE([AC_PROG_CXX])
 AC_LANG_PUSH([C++])

 AC_ARG_WITH([freeimageplus],
   [AC_HELP_STRING([--with-freeimageplus@<:@=DIR@:>@],
     [using FreeImage (DIR = prefix for FreeImage installation)])])
 FREEIMAGEPLUS_CXXFLAGS=''
 FREEIMAGEPLUS_LDFLAGS=''
 if test "x$with_freeimageplus" != xno; then
   if test -n "$with_freeimageplus"; then
     FREEIMAGEPLUS_CXXFLAGS="-I${with_freeimageplus}/include"
     FREEIMAGEPLUS_LDFLAGS="-L${with_freeimageplus}/lib"
   fi
   oln_save_CXXFLAGS=$CXXFLAGS
   oln_save_LDFLAGS=$LDFLAGS
   CXXFLAGS="$CXXFLAGS $FREEIMAGEPLUS_CXXFLAGS"
   LDFLAGS="$LDFLAGS $FREEIMAGEPLUS_LDFLAGS"
   oln_have_freeimageplus=no
   AC_CHECK_HEADER([FreeImagePlus.h],
     [AC_CHECK_LIB([freeimageplus],
       [main],
       [oln_have_freeimageplus=yes
         FREEIMAGEPLUS_LDFLAGS="$FREEIMAGEPLUS_LDFLAGS -lfreeimageplus"
         AC_DEFINE([HAVE_FREEIMAGEPLUS], 1,
                   [Define to 1 if we can use FreeImage])
       ]
     )]
   )
   CXXFLAGS=$oln_save_CXXFLAGS
   LDFLAGS=$oln_save_LDFLAGS
   TOOLS_LDFLAGS="$TOOLS_LDFLAGS $FREEIMAGEPLUS_LDFLAGS"
 fi
 AC_SUBST([FREEIMAGEPLUS_CXXFLAGS])
 AC_SUBST([FREEIMAGEPLUS_LDFLAGS])

 AC_LANG_POP([C++])
])
