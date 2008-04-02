## FIXME: Revamp (see Olena 0.11's test suite).

# Look for Milena header in srcdir.
AM_CPPFLAGS = -I$(top_srcdir)/milena

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 (or just a part of it) is available in the distribution,
# instead of using the hand-made TOOLS_CXXFLAGS.
TOOLS_CXXFLAGS = @TOOLS_CXXFLAGS@
AM_CXXFLAGS = $(TOOLS_CXXFLAGS)
