# Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development
# Laboratory (LRDE).
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

## Process this file through Automake to produce Makefile.in.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

#<<lrde
if ENABLE_SCRIBO
  SUBDIRS += scribo
endif ENABLE_SCRIBO
#>>

# Target shortcuts delegating the actual action to milena/Makefile.
LOCAL_RECURSIVE_TARGETS = doc pretty-check tests
$(LOCAL_RECURSIVE_TARGETS):
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: $(LOCAL_RECURSIVE_TARGETS)

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += milena scribo


#<<lrde
# Unless Make variable MAINTAINER is set to `true' (e.g. by calling
# `make dist MAINTAINER=true'), do not distribute parts tagged `lrde'
# (see build-aux/strip-lrde).
#
# Remove any trace of olena/oln/lrde from the distribution and reset
# the GNU build system for the distdir layout.
dist-hook:
	if test x$(MAINTAINER) != xtrue; then				\
	  $(top_srcdir)/build-aux/strip-lrde $(distdir)/configure.ac &&	\
	  $(top_srcdir)/build-aux/strip-lrde $(distdir)/Makefile.am &&	\
	  rm -rf $(distdir)/scribo &&					\
	  (								\
	    cd $(distdir);						\
	    set -evx;							\
	    autoreconf -f -v -i						\
	    find . -name autom4te.cache | xargs rm -rf			\
	  )								\
	fi
#>>
