## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/static -I$(top_srcdir)/metalic

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution, instead of using the 
# set-by-hand TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)


check_PROGRAMS =				\
  any						\
  entry						\
  exact						\
  find_exact					\
  super						\
  vtypes					\
  vtypes-and-exact				\
						\
  tour						\
						\
  scoop-alt


any_SOURCES                    = any.cc
entry_SOURCES                  = entry.cc
exact_SOURCES                  = exact.cc
find_exact_SOURCES             = find_exact.cc
super_SOURCES                  = super.cc
vtypes_SOURCES                 = vtypes.cc
vtypes_and_exact_SOURCES       = vtypes-and-exact.cc

tour_SOURCES = tour.cc

scoop_alt_SOURCES = scoop-alt.cc

# Old tests.
EXTRA_DIST = multiple-supers.cc vtypes-multiple-supers.cc

TESTS = $(check_PROGRAMS)
