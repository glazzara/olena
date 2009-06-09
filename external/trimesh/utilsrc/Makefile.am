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

# Linking with gluit and GLUT/OpenGL.
gluit_builddir = $(top_builddir)/gluit
include $(top_srcdir)/gluit/gluit.mk


AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = $(GLFLAGS)
LDADD = -L$(top_builddir)/libsrc -ltrimesh -lm $(GLLIBS)


bin_PROGRAMS =					\
  mesh_align					\
  mesh_cat					\
  mesh_cc					\
  mesh_filter					\
  mesh_make					\
  mesh_shade					\
  mesh_view

mesh_align_SOURCES = mesh_align.cc
mesh_cat_SOURCES = mesh_cat.cc
mesh_cc_SOURCES = mesh_cc.cc
mesh_filter_SOURCES = mesh_filter.cc
mesh_make_SOURCES = mesh_make.cc
mesh_shade_SOURCES = mesh_shade.cc
mesh_view_SOURCES = mesh_view.cc
