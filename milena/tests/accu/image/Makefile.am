## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  init						\
  set_value					\
  take						\
  take_as_init					\
  take_n_times					\
  to_result

init_SOURCES = init.cc
set_value_SOURCES = set_value.cc
take_SOURCES = take.cc
take_as_init_SOURCES = take_as_init.cc
take_n_times_SOURCES = take_n_times.cc
to_result_SOURCES = to_result.cc

TESTS = $(check_PROGRAMS)
