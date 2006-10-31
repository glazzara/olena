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
  dpoint2d					\
  point2d					\
  grid						\
  image_entry					\
  image1d					\
  image2d					\
  image3d					\
  npoints					\
  pw_value					\
  window2d					\
						\
  at

# Images and auxiliary structures.
dpoint2d_SOURCES = dpoint2d.cc
point2d_SOURCES = point2d.cc
grid_SOURCES = grid.cc
image_entry_SOURCES = image_entry.cc
image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
npoints_SOURCES = npoints.cc
pw_value_SOURCES = pw_value.cc
window2d_SOURCES = window2d.cc

# Methods.
at_SOURCES = at.cc


TESTS = $(check_PROGRAMS)
