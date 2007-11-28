## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  println					\
  println_with_border				\
  iota

println_SOURCES = println.cc
println_with_border_SOURCES = println_with_border.cc
iota_SOURCES = iota.cc

TESTS = $(check_PROGRAMS)
