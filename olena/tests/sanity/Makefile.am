## Process this file through Automake to create Makefile.in -*- Makefile -*-

EXTRA_DIST = includes.test

# FIXME: These should be defined by oln.m4.
OLN_LOCAL_SRC = $(top_srcdir)/olena
OLN_LOCAL_BUILD = $(top_builddir)/olena
# FIXME: Likewise.
OLN_LOCAL_CPPFLAGS = -I$(top_srcdir)/olena    -I$(top_builddir)/olena
XTD_LOCAL_CPPFLAGS = -I$(top_srcdir)/extended -I$(top_builddir)/extended
STC_LOCAL_CPPFLAGS = -I$(top_srcdir)/static   -I$(top_builddir)/static
MLC_LOCAL_CPPFLAGS = -I$(top_srcdir)/metalic  -I$(top_builddir)/metalic
CPPFLAGS = \
  $(OLN_LOCAL_CPPFLAGS) $(XTD_LOCAL_CPPFLAGS) \
  $(STC_LOCAL_CPPFLAGS) $(MLC_LOCAL_CPPFLAGS)

check:
	TOP_SRCDIR=$(top_srcdir) SRCDIR=$(srcdir) \
	TOP_BUILDDIR=$(top_builddir) \
	PATH_TO_OLN_SRCDIR=$(OLN_LOCAL_SRC) \
	PATH_TO_OLN_BUILDDIR=$(OLN_LOCAL_BUILD) \
	CPPFLAGS="$(CPPFLAGS) $(DEFS) -I$(top_builddir)" \
	  $(SHELL) $(srcdir)/includes.test

CLEANFILES = failures incltest.*
