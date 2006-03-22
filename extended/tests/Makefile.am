## Process this file through Automake to create Makefile.in -*- Makefile -*-

AM_CPPFLAGS = -I$(top_srcdir)/extended
# FIXME: Add 
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_OPTIMIZE) -ggdb
#
# when oln.m4 is available in the distribution.

check_PROGRAMS =				\
  id						\
  cast						\
  cos

id_SOURCES = id.cc
cast_SOURCES = cast.cc
cos_SOURCES = cos.cc

TESTS = $(check_PROGRAMS)
