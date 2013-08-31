# Copyright (C) 2010-2013 EPITA Research and Development Laboratory (LRDE).
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

include $(top_srcdir)/milena/common.mk

# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
# Find apps/data.hh (generated file).
AM_CPPFLAGS += -I$(top_builddir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

# Routines used in the computation of binary skeletons of 3D images.
# FIXME: To be integrated into the library.
noinst_HEADERS = image3d-skel.hh

# FIXME: Really install these programs?
# 2D cases.
bin_PROGRAMS =					\
  image2d-skel-unconstrained			\
  image2d-skel-with-end-points

# 3D cases, slow versions.
bin_PROGRAMS +=					\
  image3d-skel-unconstrained			\
  image3d-skel-with-end-points			\
  image3d-skel-with-1d-isthmuses

EXTRA_DIST =					\
  test-image2d-skel-unconstrained.in		\
  test-image2d-skel-with-end-points.in

# Use Make to generate Doxyfile instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed -e 's|@top_srcdir[@]|$(top_srcdir)|g'

# Tests for the 2D case.
generated_tests =				\
  test-image2d-skel-unconstrained		\
  test-image2d-skel-with-end-points

$(generated_tests): Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

test-image2d-skel-unconstrained: $(srcdir)/test-image2d-skel-unconstrained.in
test-image2d-skel-with-end-points: $(srcdir)/test-image2d-skel-with-end-points.in

CLEANFILES = $(generated_tests)

TESTS = $(generated_tests)

# Tests for the 3D case.
# FIXME: Create real test cases with inputs passed as argument.
TESTS +=					\
  image3d-skel-unconstrained			\
  image3d-skel-with-end-points			\
  image3d-skel-with-1d-isthmuses

# FIXME: Rename all outputs to have their names use their generator as prefix.
MOSTLYCLEANFILES =				\
  image3d-skel-unconstrained-input.raw		\
  image3d-skel-unconstrained-input.vtk		\
  image3d-skel-unconstrained-skel.raw		\
  image3d-skel-unconstrained-skel.vtk		\
  image3d-skel-with-end-points-input.raw	\
  image3d-skel-with-end-points-input.vtk	\
  image3d-skel-with-end-points-skel.raw		\
  image3d-skel-with-end-points-skel.vtk		\
  image3d-skel-with-1d-isthmuses-input.raw	\
  image3d-skel-with-1d-isthmuses-input.vtk	\
  image3d-skel-with-1d-isthmuses-skel.raw	\
  image3d-skel-with-1d-isthmuses-skel.vtk	\
  picasso-skel-unconstrained.pbm		\
  picasso-skel-with-end-points.pbm		\
  tiny-skel-unconstrained.pbm			\
  tiny-skel-with-end-points.pbm

# Mixed tests corresponding to illustrations of the WADGMM 2010/LNCS
# submission.  Not part of the `check' target; triggered by the
# `check-mixed' target.
generated_tests += test-mixed
test-mixed: $(srcdir)/test-mixed.in
.PHONY: check-mixed
check-mixed: test-mixed $(bin_PROGRAMS)
	./test-mixed
