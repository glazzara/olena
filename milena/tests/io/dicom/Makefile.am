## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS = dicom

# FIXME: Upon completion, if this test only read (not write) images,
# rename it as load.cc
dicom_SOURCES = dicom.cc

TESTS = $(check_PROGRAMS)
