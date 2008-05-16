## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
   show						\
   save_and_show				\
   color_pretty

show_SOURCES = show.cc
save_and_show_SOURCES = save_and_show.cc
color_pretty_SOURCES = color_pretty.cc

# FIXME: Don't execute `save' and `save_and_show' by default!  These
# tests require a running X11 server (as well as a `pkill' binary...).
# Perform the necessary checks at configure time first.
#TESTS = $(check_PROGRAMS)
TESTS = color_pretty
