## FIXME: Revamp (see Olena 0.11's test suite).

AM_CPPFLAGS = -I$(top_srcdir)/milena

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 is available in the distribution, instead of using the
# set-by-hand TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)
