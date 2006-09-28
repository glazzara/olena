## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS =						\
  -I$(top_srcdir)/olena -I$(top_srcdir)/extended	\
  -I$(top_srcdir)/static -I$(top_srcdir)/metalic

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution.
# Meanwhile, alter CXXFLAGS to turn off any optimization.
CXXFLAGS = -O0 -ggdb


check_PROGRAMS =				\
  grid						\
  image_entry					\
  npoints					\
						\
  identity_morpher				\
  add_neighborhood_morpher			\
  morphers					\
						\
  fill

# Images and auxiliary structures.
grid_SOURCES = grid.cc
image_entry_SOURCES = image_entry.cc
npoints_SOURCES = npoints.cc

# Morphers.
identity_morpher_SOURCES = identity_morpher.cc
add_neighborhood_morpher_SOURCES = add_neighborhood_morpher.cc
morphers_SOURCES = morphers.cc

# Algorithms.
fill_SOURCES = fill.cc


TESTS = $(check_PROGRAMS)
