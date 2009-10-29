#                                                       -*- Autoconf -*-

# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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


# DYN_LIBIBERTY
# -------------
# Look for libiberty.
AC_DEFUN([DYN_LIBIBERTY],
[AC_LANG_PUSH([C++])dnl
# Look for cplus_demangle, which is the only function we use from libiberty.
AC_CHECK_LIB([iberty], [cplus_demangle],, 
	     [AC_MSG_ERROR([
Cannot link with libiberty.  If the library is installed in an unusual
path on your system, please run configure with suitable LDFLAGS.  For
instance, if it is installed in /opt/local/lib, use:

  configure LDFLAGS="-L/opt/local/lib"
])])
 LIBS="$LIBS -liberty"
 AC_LANG_POP([C++])dnl
])
