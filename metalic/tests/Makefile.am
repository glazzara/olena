## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/metalic
# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) -ggdb
#
# when oln.m4 is available in the distribution.
AM_CXXFLAGS = -O0 -ggdb


check_PROGRAMS =				\
  case						\
  gcase						\
  if						\
  is_a						\
  or						\
  protected					\
  ret						\
  switch					\
  typedef

case_SOURCES      = case.cc
gcase_SOURCES     = gcase.cc
if_SOURCES        = if.cc
is_a_SOURCES      = is_a.cc
or_SOURCES        = or.cc
protected_SOURCES = protected.cc
ret_SOURCES       = ret.cc
switch_SOURCES    = switch.cc
typedef_SOURCES   = typedef.cc

TESTS = $(check_PROGRAMS)
