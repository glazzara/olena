# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
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

# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)


bin_PROGRAMS = image2d-skel-unconstrained image2d-skel-with-end-points
image2d_skel_unconstrained_SOURCES = image2d-skel-unconstrained.cc image2d-skel.hh
image2d_skel_with_end_points_SOURCES = image2d-skel-with-end-points.cc image2d-skel.hh

EXTRA_DIST = test-image2d-skel-unconstrained.in test-image2d-skel-with-end-points.in

# Use Make to generate Doxyfile instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed -e 's|@top_srcdir[@]|$(top_srcdir)|g'

test-image2d-skel-unconstrained test-image2d-skel-with-end-points: Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

test-image2d-skel-unconstrained: $(srcdir)/test-image2d-skel-unconstrained.in
test-image2d-skel-with-end-points: $(srcdir)/test-image2d-skel-with-end-points.in

TESTS = test-image2d-skel-unconstrained test-image2d-skel-with-end-points
CLEANFILES = test-image2d-skel-unconstrained test-image2d-skel-with-end-points

MOSTLYCLEANFILES =				\
  picasso-skel-unconstrained.pbm		\
  picasso-skel-with-end-points.pbm		\
  tiny-skel-unconstrained.pbm			\
  tiny-skel-with-end-points.pbm
