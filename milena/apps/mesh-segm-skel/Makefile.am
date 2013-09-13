# Copyright (C) 2008, 2009, 2010, 2012, 2013 EPITA Research and Development
# Laboratory (LRDE)
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

include $(top_srcdir)/milena/apps/apps.mk

noinst_PROGRAMS =
generated_checked_tests =
generated_tests = $(generated_checked_tests)
EXTRA_DIST =
TESTS =
MOSTLYCLEANFILES =

# Use Make to generate tests instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed							\
         -e 's|@top_srcdir[@]|$(top_srcdir)|g'			\
         -e 's|@abs_srcdir[@]|$(abs_srcdir)|g'			\
         -e 's|@abs_builddir[@]|$(abs_builddir)|g'		\
         -e 's|@abs_top_builddir[@]|$(abs_top_builddir)|g'

$(generated_tests): Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(edit) $${srcdir}$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

# FIXME: Have program names embed the type of structure used (either
# graph or complex).

## --------------------------- ##
## Graph-based applications.   ##
## --------------------------- ##

# FIXME: To be written (and factored, of course):
#
#  mesh-graph-{max,pinv}-curv
#
#  mesh-graph-segm
#  mesh-graph-{max,pinv}-curv-segm
#
#  mesh-graph-skel
#  mesh-graph-{max,pinv}-curv-skel
#
#  ...

## ---------------------------- ##
## Complex-based applications.  ##
## ---------------------------- ##

# A Milena re-implementation of some features from Trimesh.  This code
# is completely independent from the Trimesh library.
# FIXME: Integrate this header into Milena.
noinst_HEADERS = misc.hh

## Curvature computation.
## ----------------------

# A small program exercising the curvature computation routines ported
# from Trimesh to Milena.
noinst_PROGRAMS += mesh-complex-max-curv
TESTS += test-mesh-complex-max-curv
MOSTLYCLEANFILES += teapot-complex-max-curv.off

# FIXME: Implement (and exercise) mesh-complex-pinv-curv.

# A program computing the max curvature at each (2-)face of the
# surface of a triangle mesh and displaying its local minima
# (in blue) and maxima (in red).
noinst_PROGRAMS += mesh-complex-max-curv-extrema
TESTS += test-mesh-complex-max-curv-extrema
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-extrema.off		\
  teapot-complex-max-curv-extrema.off		\
  bunny-holefilled-complex-max-curv-extrema.off

# FIXME: Implement (and exercise) mesh-complex-pinv-curv-extrema.

## Segmentation.
## -------------

# Segmentation program working on precomputed meshes with curvature
# data.  Less accurate, since curvature is stored on triangles
# (polygons), whereas the segmentation is performed on edges.
noinst_PROGRAMS += mesh-complex-segm
TESTS += test-mesh-complex-segm
MOSTLYCLEANFILES +=				\
  socket-max-curv-segm.off			\
  teapot-max-curv-segm.off			\
  bunny-holefilled-pinv-curv-segm.off

# Program computing the max curvature from a mesh, then segmenting it.
# The advantage of this program over `mesh-complex-segm' is that the
# curvature information is much more precise, as data is computed on
# vertices, then average values are computed on edges (in the case of
# `mesh-complex-segm', data are read from polygons, which implies an
# extra averaging step).
noinst_PROGRAMS += mesh-complex-max-curv-segm
TESTS += test-mesh-complex-max-curv-segm
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-segm.off		\
  teapot-complex-max-curv-segm.off		\
  bunny-holefilled-complex-max-curv-segm.off

# Likewise, but using pseudo-inverse curvature.
noinst_PROGRAMS += mesh-complex-pinv-curv-segm
TESTS += test-mesh-complex-pinv-curv-segm
MOSTLYCLEANFILES +=				\
  socket-complex-pinv-curv-segm.off		\
  teapot-complex-pinv-curv-segm.off		\
  bunny-holefilled-complex-pinv-curv-segm.off

## Collapses.
## ----------

# Performing a 2-collapse on a pseudo-manifold.
noinst_PROGRAMS += mesh-complex-2-collapse

generated_checked_tests += test-mesh-complex-2-collapse
EXTRA_DIST += test-mesh-complex-2-collapse.in
test-mesh-complex-2-collapse: $(srcdir)/test-mesh-complex-2-collapse.in
MOSTLYCLEANFILES += pseudo-manifold-2-collapse.vtk

# Program computing the max curvature from a mesh, then performing a
# 2-collapse on it.
noinst_PROGRAMS += mesh-complex-max-curv-2-collapse

generated_checked_tests += test-mesh-complex-max-curv-2-collapse
EXTRA_DIST += test-mesh-complex-max-curv-2-collapse.in
test-mesh-complex-max-curv-2-collapse: $(srcdir)/test-mesh-complex-max-curv-2-collapse.in
MOSTLYCLEANFILES +=				\
  teapot-max-curv-2-collapse.vtk		\
  bunny-holefilled-max-curv-2-collapse.vtk

# Program computing the max curvature from a mesh, then performing a
# 1-collapse on it.
noinst_PROGRAMS += mesh-complex-max-curv-1-collapse

generated_checked_tests += test-mesh-complex-max-curv-1-collapse
EXTRA_DIST += test-mesh-complex-max-curv-1-collapse.in
test-mesh-complex-max-curv-1-collapse: $(srcdir)/test-mesh-complex-max-curv-1-collapse.in
MOSTLYCLEANFILES +=				\
  teapot-max-curv-1-collapse.vtk		\
  bunny-holefilled-max-curv-1-collapse.vtk

## More complex skeletonization chains.
## ------------------------------------

noinst_HEADERS += save_bin_alt.hh

# Skeletonization program working on precomputed meshes with curvatures data.
noinst_PROGRAMS += mesh-complex-skel
TESTS += test-mesh-complex-skel
MOSTLYCLEANFILES +=				\
  socket-max-curv-skel.off			\
  teapot-max-curv-skel.off			\
  three-triangles-skel.off			\
  bunny-holefilled-pinv-curv-skel.off

noinst_PROGRAMS += mesh-complex-max-curv-skel
TESTS += test-mesh-complex-max-curv-skel
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-skel.off		\
  teapot-complex-max-curv-skel.off		\
  bunny-holefilled-complex-max-curv-skel.off

noinst_PROGRAMS += mesh-complex-pinv-curv-skel
TESTS += test-mesh-complex-pinv-curv-skel
MOSTLYCLEANFILES +=				\
  socket-complex-pinv-curv-skel.off		\
  teapot-complex-pinv-curv-skel.off		\
  bunny-holefilled-complex-pinv-curv-skel.off

## ------------------------- ##
## Conversion applications.  ##
## ------------------------- ##

# Convert a binary OFF file (e.g. no value attached to faces) into a
# VTK file.
noinst_PROGRAMS += off-to-vtk-bin

## ------------- ##
## Mixed tests.  ##
## ------------- ##

# Mixed tests corresponding to illustrations of the WADGMM 2010/LNCS
# paper.  Not part of the `check' target; triggered by the
# `check-mixed' target.
generated_tests += test-mixed
EXTRA_DIST += test-mixed.in
test-mixed: $(srcdir)/test-mixed.in
.PHONY: check-mixed
check-mixed: test-mixed $(noinst_PROGRAMS)
	./test-mixed
MOSTLYCLEANFILES +=						\
  bunny-holefilled-complex-pinv-curv-skel-wadgmm-2010.off	\
  bunny-holefilled-max-curv-2-collapse-wadgmm-2010.vtk		\
  bunny-holefilled-max-curv-1-collapse-wadgmm-2010.vtk


# Check tests from generated_checked_tests (which is a subset of
# generated_tests).
TESTS += $(generated_checked_tests)
CLEANFILES = $(generated_tests)
