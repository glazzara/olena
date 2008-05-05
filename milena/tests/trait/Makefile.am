## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = image op value

check_PROGRAMS =				\
  ch_value					\
  super

ch_value_SOURCES = ch_value.cc
super_SOURCES = super.cc

TESTS = $(check_PROGRAMS)
