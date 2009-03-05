## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS =					\
  approx

check_PROGRAMS =			\
	leveling			\
	algebraic			\
	height				\
	area				\
	volume				\
	structural			\
	sum

leveling_SOURCES = leveling.cc
algebraic_SOURCES = algebraic.cc
height_SOURCES = height.cc
area_SOURCES = area.cc
volume_SOURCES = volume.cc
structural_SOURCES = structural.cc
sum_SOURCES = sum.cc

TESTS = $(check_PROGRAMS)
