# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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
#

## Process this file through Automake to produce Makefile.in.

SUBDIRS = include libsrc gluit utilsrc

# FIXME: These files are not used with the new (Autotool'd) build
# system.  We keep them until all their parameters have been
# transferred to configure.ac and Makefile.am's (e.g., support for
# Win32/Cygwin, etc.)
EXTRA_DIST =					\
  Makedefs.Darwin				\
  Makedefs.Linux				\
  Makedefs.Linux64				\
  Makedefs.SunOS				\
  Makedefs.Win32				\
  Makerules

# FIXME: Convert this to Automake.
# debug:
# 	$(MAKE) -C libsrc DEBUG=y
# 	$(MAKE) -C gluit DEBUG=y
# 	$(MAKE) -C utilsrc DEBUG=y
