## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  morph 					\
  dmorph 					\
  imorph 					\
  vmorph

##FIXME: re-enable tests
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

##  bgraph_image					\
##  mono_obased_rle_image				\
##  mono_rle_image				\
##  obased_rle_image				\
##  rle_image					\
##  sparse_image					\
##  value_enc_image



noinst_HEADERS = complex_image.hh

##bgraph_image_SOURCES = bgraph_image.cc
complex_image_SOURCES = complex_image.cc
graph_image_SOURCES = graph_image.cc
flat_image_SOURCES = flat_image.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
line_graph_image_SOURCES = line_graph_image.cc
##mono_obased_rle_image_SOURCES = mono_obased_rle_image.cc
##mono_rle_image_SOURCES = mono_rle_image.cc
##obased_rle_image_SOURCES = obased_rle_image.cc
##rle_image_SOURCES = rle_image.cc
##sparse_image_SOURCES = sparse_image.cc
##value_enc_image_SOURCES = value_enc_image.cc
vertex_image_SOURCES = vertex_image.cc
edge_image_SOURCES = edge_image.cc

# Tests depending on the FreeImagePlus library.
if FREEIMAGEPLUS
  check_PROGRAMS += fi_adaptor
  fi_adaptor_SOURCES = fi_adaptor.cc
  fi_adaptor_CPPFLAGS = $(AM_CPPFLAGS) $(FREEIMAGEPLUS_CPPFLAGS)
  fi_adaptor_LDFLAGS = $(AM_LDFLAGS) $(FREEIMAGEPLUS_LDFLAGS)
endif

TESTS = $(check_PROGRAMS)
