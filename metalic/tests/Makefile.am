## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/metalic
# FIXME: Add 
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  if						\
  is_a						\
  properties					\
  typedef

if_SOURCES = if.cc
is_a_SOURCES = is_a.cc
properties_SOURCES = properties.cc
typedef_SOURCES = typedef.cc

TESTS = $(check_PROGRAMS)
