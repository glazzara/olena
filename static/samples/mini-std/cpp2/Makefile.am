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


check_PROGRAMS = mini-std
mini_std_SOURCES = mini-std.cc

TESTS = $(check_PROGRAMS)
