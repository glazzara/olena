# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
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

## Process this file through Automake to create Makefile.in.

SUBDIRS = tests src

.PHONY: bin

bin:
	make -C src

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += tests

# FIXME: the generation should be done from scribo's directory.
regen-am:
	cd $(srcdir) && 				\
	$(top_srcdir)/build-aux/generate_dist_headers.sh\
		    .	 				\
		    ./headers.mk 			\
		    ./nodist-headers

# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(top_srcdir)/build-aux/generate_dist_headers.sh
	cd $(srcdir) && 				\
	$(top_srcdir)/build-aux/generate_dist_headers.sh\
		    . 					\
		    ./headers.mk 			\
		    ./nodist-headers

include $(srcdir)/headers.mk
