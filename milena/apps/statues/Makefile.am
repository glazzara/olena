## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# Linking with gluit and GLUT/OpenGL.
gluit_builddir = $(top_builddir)/external/trimesh/gluit
include $(top_srcdir)/external/trimesh/gluit/gluit.mk


# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_srcdir)/external/trimesh/include
# Produce fast code.
AM_CXXFLAGS = -O3 -DNDEBUG -ggdb
# Find the trimesh library and its dependencies.
AM_LDFLAGS = $(GLFLAGS)
LDADD = -L$(top_builddir)/external/trimesh/libsrc -ltrimesh $(GLLIBS)


bin_PROGRAMS = mesh-pinv-curv mesh-max-curv mesh-segm mesh-skel

mesh_pinv_curv_SOURCES = mesh-pinv-curv.cc io.hh
mesh_max_curv_SOURCES = mesh-max-curv.cc io.hh
mesh_segm_SOURCES = mesh-segm.cc io.hh
mesh_skel_SOURCES = mesh-skel.cc io.hh

TESTS = test
