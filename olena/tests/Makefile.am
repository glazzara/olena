## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS =						\
  -I$(top_srcdir)/olena -I$(top_srcdir)/extended	\
  -I$(top_srcdir)/static -I$(top_srcdir)/metalic
# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  grid						\
  identity_morpher				\
  image_entry					\
  morphers					\
  npoints

grid_SOURCES = grid.cc
identity_morpher_SOURCES = identity_morpher.cc
image_entry_SOURCES = image_entry.cc
npoints_SOURCES = npoints.cc
morphers_SOURCES = morphers.cc

TESTS = $(check_PROGRAMS)
