## Process this file through Automake to create Makefile.in -*- Makefile -*-

SUBDIRS = check sanity

AM_CPPFLAGS =						\
  -I$(top_srcdir)/olena -I$(top_srcdir)/extended	\
  -I$(top_srcdir)/static -I$(top_srcdir)/metalic

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution.
# Meanwhile, alter CXXFLAGS to turn off any optimization (Automake will
# warn against this, but that's OK).
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
CXXFLAGS += $(TESTS_CXXFLAGS)


check_PROGRAMS =				\
  dpoint2d					\
  point2d					\
  grid						\
  image_entry					\
  image1d					\
  image2d					\
  image3d					\
  io_pnm					\
  npoints					\
						\
  identity_morpher				\
  add_neighborhood_morpher			\
  morphers					\
						\
  fill

# Images and auxiliary structures.
dpoint2d_SOURCES = dpoint2d.cc
point2d_SOURCES = point2d.cc
grid_SOURCES = grid.cc
image_entry_SOURCES = image_entry.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
npoints_SOURCES = npoints.cc

# Morphers.
identity_morpher_SOURCES = identity_morpher.cc
add_neighborhood_morpher_SOURCES = add_neighborhood_morpher.cc
morphers_SOURCES = morphers.cc

# Algorithms.
fill_SOURCES = fill.cc


TESTS = $(check_PROGRAMS)
