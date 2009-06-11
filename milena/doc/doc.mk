## FIXME: Revamp (see Olena 0.11's test suite).		-*- Automake -*-

include $(top_srcdir)/milena/tests/tests.mk

AM_CPPFLAGS += -I$(top_srcdir)/milena/doc/tutorial/tools


DOC_SRCDIR = $(top_srcdir)/milena/doc/
OUTPUTS_SRCDIR = $(DOC_SRCDIR)/outputs
SPLIT_OUTPUTS_SRCDIR = $(OUTPUTS_SRCDIR)/split
FIGURES_SRCDIR = $(DOC_SRCDIR)/figures
EXAMPLES_SRCDIR = $(DOC_SRCDIR)/examples
SPLIT_EXAMPLES_SRCDIR = $(EXAMPLES_SRCDIR)/split
IMG_SRCDIR = $(DOC_SRCDIR)/img

DOC_BUILDDIR = $(top_builddir)/milena/doc/
OUTPUTS_BUILDDIR = $(DOC_BUILDDIR)/outputs
SPLIT_OUTPUTS_BUILDDIR = $(OUTPUTS_BUILDDIR)/split
FIGURES_BUILDDIR = $(DOC_BUILDDIR)/figures
EXAMPLES_BUILDDIR = $(DOC_BUILDDIR)/examples
SPLIT_EXAMPLES_BUILDDIR = $(EXAMPLES_BUILDDIR)/split
