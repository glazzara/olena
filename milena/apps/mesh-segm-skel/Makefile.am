# Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

## ---------------------------- ##
## Complex-based applications.  ##
## ---------------------------- ##

# A Milena re-implementation of some features from Trimesh.  This code
# is completely independent from the Trimesh library.
noinst_HEADERS = misc.hh

## Segmentation.
## ------------

# A small program exercising the curvature computation routines ported
# from Trimesh to Milena.
bin_PROGRAMS += mesh-complex-max-curv
mesh_complex_max_curv_SOURCES = mesh-complex-max-curv.cc
TESTS += test-mesh-complex-max-curv
MOSTLYCLEANFILES += teapot-complex-max-curv.off

# FIXME: Write a program showing the local minima (and maxima) of
# scalar-valued complex-based mesh (to see the curvature extrema).
# ...

# Segmentation program working on precomputed meshes with curvatures data.
bin_PROGRAMS += mesh-complex-segm
mesh_complex_segm_SOURCES = mesh-complex-segm.cc
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
mesh_complex_max_curv_segm_SOURCES = mesh-complex-max-curv-segm.cc
TESTS += test-mesh-complex-max-curv-segm
MOSTLYCLEANFILES +=				\
  socket-complex-max-curv-segm.off		\
  teapot-complex-max-curv-segm.off

# FIXME: Implement `mesh-complex-pinv-curv-segm' (factor as much
# code as possible with `mesh-complex-max-curv-segm'.
# ...

## Skeletonization.
## ----------------

# Skeletonization program working on precomputed meshes with curvatures data.
bin_PROGRAMS += mesh-complex-skel
mesh_complex_skel_SOURCES = mesh-complex-skel.cc save_bin_alt.hh
TESTS += test-mesh-complex-skel
MOSTLYCLEANFILES +=				\
  socket-max-curv-skel.off			\
  teapot-max-curv.off				\
  three-triangles-skel.off
