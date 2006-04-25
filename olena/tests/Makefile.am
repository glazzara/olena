## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS =					\
  -I$(top_srcdir)/olena -I$(top_srcdir)/static	\
  -I$(top_srcdir)/metalic
# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  grid						\
  image_entry

image_entry_SOURCES = image_entry.cc
grid_SOURCES = grid.cc

TESTS = $(check_PROGRAMS)
