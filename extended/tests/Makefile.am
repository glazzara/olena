## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/extended -I$(top_srcdir)/metalic
# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) -ggdb
#
# when oln.m4 is available in the distribution.
AM_CXXFLAGS = -O0 -ggdb


check_PROGRAMS =				\
  abs						\
  bind						\
  cast						\
  cfun						\
  cos						\
  id						\
  lit						\
  math						\
  optraits

abs_SOURCES = abs.cc
bind_SOURCES = bind.cc
cast_SOURCES = cast.cc
cfun_SOURCES = cfun.cc
cos_SOURCES = cos.cc
id_SOURCES = id.cc
lit_SOURCES = lit.cc
math_SOURCES = math.cc
optraits_SOURCES = optraits.cc

TESTS = $(check_PROGRAMS)
