## Process this file through Automake to create Makefile.in -*- Makefile -*-

## FIXME: Revamp (see Olena 0.10's test suite).

AM_CPPFLAGS =						\
  -I$(srcdir)/.. -I$(srcdir)/../check			\
  -I$(top_srcdir)/olena -I$(top_srcdir)/extended	\
  -I$(top_srcdir)/static -I$(top_srcdir)/metalic	\
  -DOLN_IMG_DIR=\"$(top_srcdir)/olena/img\"

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
  identity_morpher				\
  add_neighborhood_morpher			\
  count_rw_morpher				\
  slice_morpher					\
  stack_morpher					\
  two_way_morpher				\
  value_cast					\
  with_lut					\
						\
  morphers

# Morphers.
identity_morpher_SOURCES = identity_morpher.cc
add_neighborhood_morpher_SOURCES = add_neighborhood_morpher.cc
count_rw_morpher_SOURCES = count_rw_morpher.cc
slice_morpher_SOURCES = slice_morpher.cc
stack_morpher_SOURCES = stack_morpher.cc
two_way_morpher_SOURCES = two_way_morpher.cc
value_cast_SOURCES = value_cast.cc
with_lut_SOURCES = with_lut.cc

morphers_SOURCES = morphers.cc


TESTS = $(check_PROGRAMS)
