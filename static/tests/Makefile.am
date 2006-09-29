## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/static -I$(top_srcdir)/metalic

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
  any						\
  entry						\
  exact						\
  find_exact					\
  multiple-supers				\
  super						\
  vtypes					\
  vtypes-and-exact				\
  vtypes-multiple-supers			\
						\
  tour

any_SOURCES                    = any.cc
entry_SOURCES                  = entry.cc
exact_SOURCES                  = exact.cc
find_exact_SOURCES             = find_exact.cc
multiple_supers_SOURCES        = multiple-supers.cc
super_SOURCES                  = super.cc
vtypes_SOURCES                 = vtypes.cc
vtypes_and_exact_SOURCES       = vtypes-and-exact.cc
vtypes_multiple_supers_SOURCES = vtypes-multiple-supers.cc

tour_SOURCES = tour.cc

TESTS = $(check_PROGRAMS)
