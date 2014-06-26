# Copyright (C) 2009, 2010, 2011, 2012, 2014 EPITA Research and Development
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

SUBDIRS = demo doc tests src

.PHONY: bin
bin:
	cd src && $(MAKE) $(AM_MAKEFLAGS) $@

.PHONY: tests
tests:
	cd tests && $(MAKE) $(AM_MAKEFLAGS) $@

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += tests

regen-am:
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       scribo headers.mk

# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$(top_srcdir)/build-aux/generate_dist_headers.sh \
#         $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(top_srcdir)/build-aux/generate_dist_headers.sh
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       scribo headers.mk

include $(srcdir)/headers.mk
