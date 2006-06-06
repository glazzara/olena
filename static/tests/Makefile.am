## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/static -I$(top_srcdir)/metalic
# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  any						\
  entry						\
  exact						\
  multiple-supers				\
  super						\
  vtypes					\
  vtypes-and-exact				\
  vtypes-multiple-supers

any_SOURCES    = any.cc
entry_SOURCES  = entry.cc
exact_SOURCES  = exact.cc
multiple_supers_SOURCES = multiple-supers.cc
super_SOURCES  = super.cc
vtypes_SOURCES = vtypes.cc
vtypes_and_exact_SOURCES = vtypes-and-exact.cc
vtypes_multiple_supers_SOURCES = vtypes-multiple-supers.cc

TESTS = $(check_PROGRAMS)
