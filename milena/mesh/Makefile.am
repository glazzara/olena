# Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

## This Makefile makes use of GNU Make extensions (%-patterns,
## patsubst).  We might want to turn it into something more portable
## when Olena is released.

meshdir = $(pkgdatadir)/meshes
dist_mesh_DATA =

## ------------ ##
## OFF Meshes.  ##
## ------------ ##

# The `$(srcdir)/' prefixes are mandatory, to force `$(pinv_curv)' and
# `$(max_curv)' to reference files living in the *source* directory,
# instead of the current (build) directory.  See their definitions
# below, using substitution references on `$(meshes)'.
meshes =					\
  $(srcdir)/bunny-holefilled.off		\
  $(srcdir)/cone.off				\
  $(srcdir)/cube.off				\
  $(srcdir)/socket.off				\
  $(srcdir)/teapot.off				\
  $(srcdir)/tetrahedron.off
dist_mesh_DATA += $(meshes)

# These meshes does not have curvature-valued versions.
more_meshes = 					\
  one-triangle.off				\
  two-triangles.off				\
  three-triangles.off
dist_mesh_DATA += $(more_meshes)

## ------------------------------------ ##
## Pseudo-inverse curvature of meshes.  ##
## ------------------------------------ ##

mesh_pinv_curv = $(top_builddir)/milena/apps/mesh-segm-skel/mesh-pinv-curv
mesh_pinv_curv: $(top_srcdir)/milena/apps/mesh-segm-skel/mesh-pinv-curv.cc
	cd $(top_builddir)/milena/apps/mesh-segm-skel/mesh-pinv-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_pinv_curv

# FIXME: *-pinv-curv.off files should not be regenerated
# automatically, because it really takes time.  Find a way to prevent
# Make to regen them without disabling this rule.
## $(srcdir)/%-pinv-curv.off: %.off $(mesh_pinv_curv)
##	$(mesh_pinv_curv) $< $@

pinv_curv = $(meshes:.off=-pinv-curv.off)
dist_mesh_DATA += $(pinv_curv)

## ----------------------------- ##
## Maximum curvature of meshes.  ##
## ----------------------------- ##

mesh_max_curv = $(top_builddir)/milena/apps/mesh-segm-skel/mesh-max-curv
mesh_max_curv: $(top_srcdir)/milena/apps/mesh-segm-skel/mesh-max-curv.cc
	cd $(top_builddir)/milena/apps/mesh-segm-skel/mesh-max-curv && \
	  $(MAKE) $(AM_MAKEFLAGS) mesh_max_curv

# FIXME: *-max-curv.off files should not be regenerated
# automatically, because it really takes time.  Find a way to prevent
# Make to regen them without disabling this rule.
## $(srcdir)/%-max-curv.off: %.off $(mesh_max_curv)
## 	$(mesh_max_curv) $< $@

max_curv = $(meshes:.off=-max-curv.off)
dist_mesh_DATA += $(max_curv)

.PHONY: debug
debug:
	@echo $(pinv_curv)
	@echo $(max_curv)


## ------------ ##
## VTK Meshes.  ##
## ------------ ##

# A VTK-format version of tetrahedron.off.
dist_mesh_DATA += tetrahedron.vtk

# The pseudo-manifold of figure 2a in cousty.09.iwcia.
dist_mesh_DATA += pseudo-manifold.vtk

# A VTK-format versions of OFF meshes produced with
# apps/mesh-segm-skel/off-to-vtk-bin.
dist_mesh_DATA +=				\
  bunny-holefilled.vtk				\
  teapot.vtk
