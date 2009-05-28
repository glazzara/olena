## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  decorated_image 				\
  interpolated 					\
  labeled_image 				\
  safe_image 					\
  tr_image 

decorated_image_SOURCES = decorated_image.cc
interpolated_SOURCES = interpolated.cc
labeled_image_SOURCES = labeled_image.cc
safe_image_SOURCES = safe_image.cc
tr_image_SOURCES = tr_image.cc

TESTS = $(check_PROGRAMS)
