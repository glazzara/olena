## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

check_PROGRAMS =				\
  extract_lines_with_rank 			\
  repair_lines
  
extract_lines_with_rank_SOURCES = extract_lines_with_rank.cc
repair_lines_SOURCES = repair_lines.cc

TESTS = $(check_PROGRAMS)
