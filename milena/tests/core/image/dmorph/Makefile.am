## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  hexa  					\
  image_if 					\
  image2d_h 					\
  p2p_image 					\
  slice_image					\
  sub_image					\
  transformed_image 				\
  unproject_image

hexa_SOURCES = hexa.cc
image_if_SOURCES = image_if.cc
image2d_h_SOURCES = image2d_h.cc
p2p_image_SOURCES = p2p_image.cc
slice_image_SOURCES = slice_image.cc
sub_image_SOURCES = sub_image.cc
transformed_image_SOURCES = transformed_image.cc
unproject_image_SOURCES = unproject_image.cc

TESTS = $(check_PROGRAMS)
