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
#

## Process this file through Automake to produce Makefile.in.

AM_CPPFLAGS = -I$(top_srcdir)/include

# FIXME: We should use Libtool instead.
lib_LTLIBRARIES = libtrimesh.la

libtrimesh_la_SOURCES =				\
  TriMesh_bounding.cc				\
  TriMesh_connectivity.cc			\
  TriMesh_curvature.cc				\
  TriMesh_io.cc					\
  TriMesh_grid.cc				\
  TriMesh_normals.cc				\
  TriMesh_pointareas.cc				\
  TriMesh_stats.cc				\
  TriMesh_tstrips.cc				\
  GLCamera.cc					\
  ICP.cc					\
  KDtree.cc					\
  diffuse.cc					\
  edgeflip.cc					\
  faceflip.cc					\
  filter.cc					\
  lmsmooth.cc					\
  remove.cc					\
  reorder_verts.cc				\
  subdiv.cc

# FIXME: Convert this to Automake.
# ifdef DEBUG
#   AM_CPPFLAGS += -DDEBUG
# endif
# debug:
# 	$(MAKE) DEBUG=y
