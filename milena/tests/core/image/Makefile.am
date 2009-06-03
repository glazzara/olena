## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  dmorph 					\
  imorph 					\
  vmorph

check_PROGRAMS =				\
  complex_image					\
  edge_image 					\
  flat_image					\
  graph_image					\
  image1d					\
  image2d					\
  image3d					\
  line_graph_image				\
  vertex_image


noinst_HEADERS = complex_image.hh

complex_image_SOURCES = complex_image.cc
graph_image_SOURCES = graph_image.cc
flat_image_SOURCES = flat_image.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
line_graph_image_SOURCES = line_graph_image.cc
vertex_image_SOURCES = vertex_image.cc
edge_image_SOURCES = edge_image.cc

TESTS = $(check_PROGRAMS)
