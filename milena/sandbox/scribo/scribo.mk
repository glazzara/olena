include $(top_srcdir)/milena/tests/tests.mk

# Add path to Scribo's headers
AM_CPPFLAGS += -I$(top_srcdir)/milena/sandbox/ -I$(top_builddir)/milena/sandbox/

