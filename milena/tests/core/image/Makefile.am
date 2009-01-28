## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

##FIXME: re-enable tests
check_PROGRAMS =				\
##  bgraph_image					\
  cast_image					\
  complex_image					\
  decorated_image				\
  flat_image					\
  hexa						\
  graph_image					\
  image1d					\
  image2d					\
  image2d_h					\
  image3d					\
  image_if					\
  interpolated					\
  line_graph_image				\
##  mono_obased_rle_image				\
##  mono_rle_image				\
##  obased_rle_image				\
  p2p_image					\
  plain						\
##  rle_image					\
  safe_image					\
##  sparse_image					\
  sub_image					\
  t_image					\
  tr_image					
##  value_enc_image

noinst_HEADERS = complex_image.hh

##bgraph_image_SOURCES = bgraph_image.cc
cast_image_SOURCES = cast_image.cc
complex_image_SOURCES = complex_image.cc
decorated_image_SOURCES = decorated_image.cc
graph_image_SOURCES = graph_image.cc
flat_image_SOURCES = flat_image.cc
hexa_SOURCES = hexa.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image2d_h_SOURCES = image2d_h.cc
image3d_SOURCES = image3d.cc
image_if_SOURCES = image_if.cc
interpolated_SOURCES = interpolated.cc
line_graph_image_SOURCES = line_graph_image.cc
##mono_obased_rle_image_SOURCES = mono_obased_rle_image.cc
##mono_rle_image_SOURCES = mono_rle_image.cc
##obased_rle_image_SOURCES = obased_rle_image.cc
p2p_image_SOURCES = p2p_image.cc
plain_SOURCES = plain.cc
##rle_image_SOURCES = rle_image.cc
safe_image_SOURCES = safe_image.cc
##sparse_image_SOURCES = sparse_image.cc
sub_image_SOURCES = sub_image.cc
t_image_SOURCES = t_image.cc
tr_image_SOURCES = tr_image.cc
##value_enc_image_SOURCES = value_enc_image.cc

# Tests depending on the FreeImagePlus library.
if FREEIMAGEPLUS
  check_PROGRAMS += fi_adaptor
  fi_adaptor_SOURCES = fi_adaptor.cc
  fi_adaptor_CPPFLAGS = $(AM_CPPFLAGS) $(FREEIMAGEPLUS_CPPFLAGS)
  fi_adaptor_LDFLAGS = $(AM_LDFLAGS) $(FREEIMAGEPLUS_LDFLAGS)
endif

TESTS = $(check_PROGRAMS)
