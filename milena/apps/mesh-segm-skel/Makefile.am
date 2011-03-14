# Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

include $(top_srcdir)/milena/common.mk

# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)

bin_PROGRAMS =
TESTS = 
MOSTLYCLEANFILES =

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
bin_PROGRAMS += mesh-complex-max-curv
TESTS += test-mesh-complex-max-curv
MOSTLYCLEANFILES += teapot-complex-max-curv.off

# FIXME: Implement (and exercise) mesh-complex-pinv-curv.

# A program computing the max curvature at each (2-)face of the
# surface of a triangle mesh and displaying its local minima
# (in blue) and maxima (in red).
bin_PROGRAMS += mesh-complex-max-curv-extrema
mesh_complex_max_curv_extrema_SOURCES = mesh-complex-max-curv-extrema.cc
TESTS += test-mesh-complex-max-curv-extrema
MOSTLYCLEANFILES +=					\
  socket-complex-max-curv-extrema.off		\
  teapot-complex-max-curv-extrema.off

# FIXME: Implement (and exercise) mesh-complex-pinv-curv-extrema.

## Segmentation.
## -------------

# Segmentation program working on precomputed meshes with curvature
# data.  Less accurate, since curvature is stored on triangles
# (polygons), whereas the segmentation is performed on edges.
bin_PROGRAMS += mesh-complex-segm
TESTS += test-mesh-complex-segm
MOSTLYCLEANFILES +=				\
  socket-max-curv-segm.off			\
  teapot-max-curv-segm.off

# Program computing the max curvature from a mesh, then segmenting it.
# The advantage of this program over `mesh-complex-segm' is that the
# curvature information is much more precise, as data is computed on
# vertices, then average values are computed on edges (in the case of
# `mesh-complex-segm', data are read from polygons, which implies an
# extra averaging step).
bin_PROGRAMS += mesh-complex-max-curv-segm
TESTS += test-mesh-complex-max-curv-segm
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-segm.off		\
  teapot-complex-max-curv-segm.off

# Likewise, but using pseudo-inverse curvature.
bin_PROGRAMS += mesh-complex-pinv-curv-segm
mesh_complex_pinv_curv_segm_SOURCES = mesh-complex-pinv-curv-segm.cc
TESTS += test-mesh-complex-pinv-curv-segm
MOSTLYCLEANFILES +=				\
  socket-complex-pinv-curv-segm.off		\
  teapot-complex-pinv-curv-segm.off

## Collapses.
## ----------

## FIXME: Generalize the generation of tests by make instead of
## configure.
# Use Make to generate tests instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed							\
         -e 's|@top_srcdir[@]|$(top_srcdir)|g'			\
         -e 's|@abs_srcdir[@]|$(abs_srcdir)|g'			\
         -e 's|@abs_builddir[@]|$(abs_builddir)|g'		\
         -e 's|@abs_top_builddir[@]|$(abs_top_builddir)|g'

# Performing a 2-collapse on a pseudo-manifold.
bin_PROGRAMS += mesh-complex-2-collapse
mesh_complex_2_collapse_SOURCES = mesh-complex-2-collapse.cc

test-mesh-complex-2-collapse: test-mesh-complex-2-collapse.in Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$< || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$< >>$@.tmp
	chmod a-w,a+x $@.tmp
	mv $@.tmp $@

TESTS += test-mesh-complex-2-collapse
CLEANFILES = test-mesh-complex-2-collapse
MOSTLYCLEANFILES += pseudo-manifold-2-collapse.vtk

# Program computing the max curvature from a mesh, then performing a
# 2-collapse on it.
bin_PROGRAMS += mesh-complex-max-curv-2-collapse
mesh_complex_max_curv_2_collapse_SOURCES = mesh-complex-max-curv-2-collapse.cc

# FIXME: Factor action with test-mesh-complex-2-collapse's.
test-mesh-complex-max-curv-2-collapse: test-mesh-complex-max-curv-2-collapse.in Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$< || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$< >>$@.tmp
	chmod a-w,a+x $@.tmp
	mv $@.tmp $@

TESTS += test-mesh-complex-max-curv-2-collapse
CLEANFILES += test-mesh-complex-max-curv-2-collapse
MOSTLYCLEANFILES +=				\
  teapot-max-curv-2-collapse.vtk		\
  bunny-holefilled-max-curv-2-collapse.vtk

# Program computing the max curvature from a mesh, then performing a
# 1-collapse on it.
bin_PROGRAMS += mesh-complex-max-curv-1-collapse
mesh_complex_max_curv_1_collapse_SOURCES = mesh-complex-max-curv-1-collapse.cc

# FIXME: Factor action with test-mesh-complex-1-collapse's.
test-mesh-complex-max-curv-1-collapse: test-mesh-complex-max-curv-1-collapse.in Makefile
	rm -f $@ $@.tmp
	srcdir='';				\
	  test -f ./$< || srcdir=$(srcdir)/;	\
	  $(edit) $${srcdir}$< >>$@.tmp
	chmod a-w,a+x $@.tmp
	mv $@.tmp $@

TESTS += test-mesh-complex-max-curv-1-collapse
CLEANFILES += test-mesh-complex-max-curv-1-collapse
MOSTLYCLEANFILES +=				\
  teapot-max-curv-1-collapse.vtk		\
  bunny-holefilled-max-curv-1-collapse.vtk

## More complex skeletonization chains.
## ------------------------------------

# Skeletonization program working on precomputed meshes with curvatures data.
bin_PROGRAMS += mesh-complex-skel
mesh_complex_skel_SOURCES = mesh-complex-skel.cc save_bin_alt.hh
TESTS += test-mesh-complex-skel
MOSTLYCLEANFILES +=				\
  socket-max-curv-skel.off			\
  teapot-max-curv-skel.off			\
  three-triangles-skel.off

bin_PROGRAMS += mesh-complex-max-curv-skel
mesh_complex_max_curv_skel_SOURCES =		\
  mesh-complex-max-curv-skel.cc save_bin_alt.hh
TESTS += test-mesh-complex-max-curv-skel
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-skel.off		\
  teapot-complex-max-curv-skel.off

bin_PROGRAMS += mesh-complex-pinv-curv-skel
mesh_complex_pinv_curv_skel_SOURCES =		\
  mesh-complex-pinv-curv-skel.cc save_bin_alt.hh
TESTS += test-mesh-complex-pinv-curv-skel
MOSTLYCLEANFILES +=				\
  socket-complex-pinv-curv-skel.off		\
  teapot-complex-pinv-curv-skel.off

## ------------------------- ##
## Conversion applications.  ##
## ------------------------- ##

# Convert a binary OFF file (e.g. no value attached to faces) into a
# VTK file.
bin_PROGRAMS += off-to-vtk-bin
off_to_vtk_bin_SOURCES = off-to-vtk-bin.cc
