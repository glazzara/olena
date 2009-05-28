#include $(top_srcdir)/milena/tests/tests.mk

# Add path to Scribo's headers
AM_CPPFLAGS = -I$(top_srcdir)/ -I$(top_builddir)/ -I$(top_srcdir)/milena -I$(top_builddir)/milena -DNDEBUG -O2

