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

# Linking with gluit and GLUT/OpenGL.
gluit_builddir = $(top_builddir)/external/trimesh/gluit
include $(top_srcdir)/external/trimesh/gluit/gluit.mk

# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
CPPFLAGS_trimesh = -I$(top_srcdir)/external/trimesh/include
# Produce fast code.
APPS_CXXFLAGS = @APPS_CXXFLAGS@
AM_CXXFLAGS = $(APPS_CXXFLAGS)
# Find the trimesh library and its dependencies.
#
# Don't use TRIMESH_LDFLAGS, since it looks like the LDFLAGS of the
# program `TRIMESH' and triggers Automake warnings; use
# LDFLAGS_trimesh instead.  Likewise for CPPFLAGS_trimesh and
# LDADD_trimesh, for symmetry reasons.
LDFLAGS_trimesh = $(GLFLAGS)
LDADD_trimesh = -L$(top_builddir)/external/trimesh/libsrc -ltrimesh $(GLLIBS)


bin_PROGRAMS =
TESTS = 
CLEANFILES =

# FIXME: Have program names embed the type of structure used (either
# graph or complex).

## FIXME: Likewise, programs' names should reflect a dependency on
## Trimesh, if any.

## ------------------------------------------------ ##
## Graph-based applications, depending on Trimesh.  ##
## ------------------------------------------------ ##

if ENABLE_TRIMESH
# bin_PROGRAMS += mesh-segm mesh-skel
# 
# mesh_segm_SOURCES = mesh-segm.cc io.hh
# mesh_segm_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
# mesh_segm_LDFLAGS = $(LDFLAGS_trimesh)
# mesh_segm_LDADD = $(LDADD_trimesh)
#
# TESTS += test-mesh-segm
# CLEANFILES += socket-segm.off teapot-segm.off

# FIXME: mesh_skel is unfinished.  Anyway, it should be superseded by
# another program, using no Trimesh code.

# mesh_skel_SOURCES = mesh-skel.cc io.hh
# mesh_skel_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
# mesh_skel_LDFLAGS = $(LDFLAGS_trimesh)
# mesh_skel_LDADD = $(LDADD_trimesh)
endif ENABLE_TRIMESH

## -------------------------------------------------- ##
## Complex-based applications, depending on Trimesh.  ##
## -------------------------------------------------- ##

if ENABLE_TRIMESH
  bin_PROGRAMS += mesh-pinv-curv mesh-max-curv

  mesh_pinv_curv_SOURCES = mesh-pinv-curv.cc io.hh
  mesh_pinv_curv_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
  mesh_pinv_curv_LDFLAGS = $(LDFLAGS_trimesh)
  mesh_pinv_curv_LDADD = $(LDADD_trimesh)

  mesh_max_curv_SOURCES = mesh-max-curv.cc io.hh
  mesh_max_curv_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
  mesh_max_curv_LDFLAGS = $(LDFLAGS_trimesh)
  mesh_max_curv_LDADD = $(LDADD_trimesh)
  TESTS += test-mesh-max-curv
  CLEANFILES += teapot-max-curv-skel.off
endif ENABLE_TRIMESH

## ---------------------------------------------------- ##
## Complex-based applications, independent of Trimesh.  ##
## ---------------------------------------------------- ##

# A Milena re-implementation of some features from Trimesh.  This code
# is completely independent from the Trimesh library.
# FIXME: Change the name of this directory, as it is confusing.
noinst_HEADERS = trimesh/misc.hh
EXTRA_DIST = trimesh/README

## Segmentation.
## ------------

# A small program exercising the curvature computation routines ported
# from Trimesh to Milena.
bin_PROGRAMS += mesh-complex-max-curv
mesh_complex_max_curv_SOURCES = mesh-complex-max-curv.cc
TESTS += test-mesh-complex-max-curv
CLEANFILES += teapot-complex-max-curv.off

# FIXME: Write a program showing the local minima (and maxima) of
# scalar-valued complex-based mesh (to see the curvature extrema).
# ...

# Segmentation program working on precomputed meshes with curvatures data.
bin_PROGRAMS += mesh-complex-segm
mesh_complex_segm_SOURCES = mesh-complex-segm.cc
TESTS += test-mesh-complex-segm
CLEANFILES +=					\
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
CLEANFILES +=					\
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
CLEANFILES +=					\
  socket-max-curv-skel.off			\
  teapot-max-curv.off				\
  three-triangles-skel.off
