## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# Find Milena and trimesh headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_srcdir)/external/trimesh/include
# Produce fast code.
AM_CXXFLAGS = -O3 -DNDEBUG -ggdb
# Find the trimesh archive.
LDADD = -L$(top_builddir)/external/trimesh/libsrc -ltrimesh

bin_PROGRAMS = mesh-segm mesh-skel
mesh_segm_SOURCES = mesh-segm.cc io.hh
mesh_skel_SOURCES = mesh-skel.cc io.hh

TESTS = test
