## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/extended -I$(top_srcdir)/metalic

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
  abs						\
  accessor					\
  bind						\
  cast						\
  cfun						\
  cos						\
  id						\
  lit						\
  math						\
  optraits

abs_SOURCES = abs.cc
accessor_SOURCES = accessor.cc
bind_SOURCES = bind.cc
cast_SOURCES = cast.cc
cfun_SOURCES = cfun.cc
cos_SOURCES = cos.cc
id_SOURCES = id.cc
lit_SOURCES = lit.cc
math_SOURCES = math.cc
optraits_SOURCES = optraits.cc

TESTS = $(check_PROGRAMS)
