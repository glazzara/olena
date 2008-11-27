## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  laplacian 					\
  dilation 					\
  gradient_internal 				\
  opening 					\
  closing 					\
  top_hat 					\
  gradient_external 				\
  gradient 					\
  erosion

laplacian_SOURCES = laplacian.cc
dilation_SOURCES = dilation.cc
gradient_internal_SOURCES = gradient_internal.cc
opening_SOURCES = opening.cc
closing_SOURCES = closing.cc
top_hat_SOURCES = top_hat.cc
gradient_external_SOURCES = gradient_external.cc
gradient_SOURCES = gradient.cc
erosion_SOURCES = erosion.cc

TESTS = $(check_PROGRAMS)
