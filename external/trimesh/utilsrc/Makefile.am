## Process this file through Automake to produce Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_builddir)/libsrc -L$(top_builddir)/gluit -lm -ltrimesh

# FIXME: Handle other OSes as well (Win32, SunOS, etc.)
if DARWIN
# LIBS += -lobjc
  GLLIBS = -framework GLUT -framework OpenGL -lgluit
else !DARWIN
  GLLIBS = -lgluit -lGL -lGLU -lX11 -lXmu -lXi
endif !DARWIN

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
mesh_view_LDFLAGS = $(AM_LDFLAGS) $(GLLIBS)
