## FIXME: Revamp (see Olena 0.11's test suite).

# Look for Milena header in srcdir and for (generated) test headers
# (like data.hh) in builddir.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 (or just a part of it) is available in the distribution,
# instead of using the hand-made TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)

# FIXME: Likewise, we should compute these values at configure time.
# Hard-code them for the moment.
LONG_TESTS_CXXFLAGS = -O3
