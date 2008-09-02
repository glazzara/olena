## Process this file through Automake to produce Makefile.in -*- Makefile -*-

# Linking with gluit and GLUT/OpenGL.
gluit_builddir = $(top_builddir)/gluit
include $(top_srcdir)/gluit/gluit.mk


AM_CPPFLAGS = -I$(top_srcdir)/include
LDADD = -L$(top_builddir)/libsrc -ltrimesh -lm


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
mesh_view_LDFLAGS = $(AM_LDFLAGS) $(GLFLAGS)
mesh_view_LDADD = $(LDADD) $(GLLIBS)
