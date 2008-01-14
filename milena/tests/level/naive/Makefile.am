## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  median

median_SOURCES = median.cc
# FIXME: The current median test is way too long to run.  Break it
# into two tests: a fast one, executed during `make check', compiled
# with no optimization, using a small kernel and a small image; and a
# second one, run during `make check-full', using a big kernel on a
# normal image (e.g. Lena) and full optimization (`-O3').
#
# For the moment, we just have the second test.  We run it during
# target `check', but we shall move it to target `check-full' as soon
# as a smaller and faster test is written.
median_CXXFLAGS = $(LONG_TESTS_CXXFLAGS)

TESTS = $(check_PROGRAMS)
