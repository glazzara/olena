## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# Linking with gluit and GLUT/OpenGL.
gluit_builddir = $(top_builddir)/external/trimesh/gluit
include $(top_srcdir)/external/trimesh/gluit/gluit.mk

# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena
CPPFLAGS_trimesh = -I$(top_srcdir)/external/trimesh/include
# Produce fast code.
AM_CXXFLAGS = -O3 -DNDEBUG -ggdb
# Find the trimesh library and its dependencies.
#
# Don't use TRIMESH_LDFLAGS, since it looks like the LDFLAGS of the
# program `TRIMESH' and triggers Automake warnings; use
# LDFLAGS_trimesh instead.  Likewise for CPPFLAGS_trimesh and
# LDADD_trimesh, for symmetry reasons.
LDFLAGS_trimesh = $(GLFLAGS)
LDADD_trimesh = -L$(top_builddir)/external/trimesh/libsrc -ltrimesh $(GLLIBS)

# FIXME: Have program names embed the type of structure used (either
# graph or complex).

## ------------------------------------------------ ##
## Graph-based applications, depending on Trimesh.  ##
## ------------------------------------------------ ##

# bin_PROGRAMS += mesh-segm mesh-skel
# 
# mesh_segm_SOURCES = mesh-segm.cc io.hh
# mesh_segm_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
# mesh_segm_LDFLAGS = $(LDFLAGS_trimesh)
# mesh_segm_LDADD = $(LDADD_trimesh)

# mesh_skel_SOURCES = mesh-skel.cc io.hh
# mesh_skel_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
# mesh_skel_LDFLAGS = $(LDFLAGS_trimesh)
# mesh_skel_LDADD = $(LDADD_trimesh)

## -------------------------------------------------- ##
## Complex-based applications, depending on Trimesh.  ##
## -------------------------------------------------- ##

bin_PROGRAMS += mesh-pinv-curv mesh-max-curv

mesh_pinv_curv_SOURCES = mesh-pinv-curv.cc io.hh
mesh_pinv_curv_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
mesh_pinv_curv_LDFLAGS = $(LDFLAGS_trimesh)
mesh_pinv_curv_LDADD = $(LDADD_trimesh)

mesh_max_curv_SOURCES = mesh-max-curv.cc io.hh
mesh_max_curv_CPPFLAGS = $(AM_CPPFLAGS) $(CPPFLAGS_trimesh)
mesh_max_curv_LDFLAGS = $(LDFLAGS_trimesh)
mesh_max_curv_LDADD = $(LDADD_trimesh)

## ---------------------------------------------------- ##
## Complex-based applications, independent of Trimesh.  ##
## ---------------------------------------------------- ##

noinst_HEADERS = trimesh/misc.hh
EXTRA_DIST = trimesh/README

bin_PROGRAMS += mesh-complex-segm
mesh_complex_segm_SOURCES = mesh-complex-segm.cc


TESTS = test-mesh-segm test-mesh-complex-segm
