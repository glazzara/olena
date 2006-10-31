## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/metalic

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution, instead of using the 
# set-by-hand TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)


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
