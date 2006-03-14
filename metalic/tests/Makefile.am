## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/metalic
# FIXME: Add 
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  case						\
  if						\
  is_a						\
  ret						\
  switch					\
  typedef

case_SOURCES = case.cc
if_SOURCES = if.cc
is_a_SOURCES = is_a.cc
ret_SOURCES = ret.cc
switch_SOURCES = switch.cc
typedef_SOURCES = typedef.cc

TESTS = $(check_PROGRAMS)
