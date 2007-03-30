## Process this file through Automake to create Makefile.in -*- Makefile -*-

## FIXME: Revamp (see Olena 0.10's test suite).

AM_CPPFLAGS =						\
  -I$(srcdir)/.. -I$(srcdir)/../check			\
  -I$(top_srcdir)/olena -I$(top_srcdir)/extended	\
  -I$(top_srcdir)/static -I$(top_srcdir)/metalic	\
  -DOLN_IMG_DIR=\"$(top_srcdir)/olena/img\"

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution, instead of using the
# set-by-hand TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)


check_PROGRAMS =				\
  apply						\
  at						\
  dpoint2d					\
  grid						\
  image1d					\
  image2d					\
  iter_point1d					\
  iter_point2d					\
  neighb2d					\
  npoints					\
  point2d					\
  pw_value					\
  rle_image					\
  sparse_image					\
  subset					\
  window2d


# Images and auxiliary structures.
dpoint2d_SOURCES = dpoint2d.cc
grid_SOURCES = grid.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
iter_point1d_SOURCES = iter_point1d.cc
iter_point2d_SOURCES = iter_point2d.cc
neighb2d_SOURCES = neighb2d.cc
npoints_SOURCES = npoints.cc
point2d_SOURCES = point2d.cc
pw_value_SOURCES = pw_value.cc
rle_image_SOURCES = rle_image.cc
sparse_image_SOURCES = sparse_image.cc
subset_SOURCES = subset.cc
window2d_SOURCES = window2d.cc

# Methods.
at_SOURCES = at.cc

# FIXME: Temp!
apply_SOURCES = apply.cc


TESTS = $(check_PROGRAMS)
