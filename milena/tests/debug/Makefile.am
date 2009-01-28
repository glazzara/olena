## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  println					\
  println_with_border				\
  iota						\
  slices_2d

println_SOURCES = println.cc
println_with_border_SOURCES = println_with_border.cc
iota_SOURCES = iota.cc
slices_2d_SOURCES = slices_2d.cc

TESTS = $(check_PROGRAMS)
