## FIXME: Revamp (see Olena 0.11's test suite).		-*- Automake -*-

include $(top_srcdir)/milena/tests/tests.mk

AM_CPPFLAGS += -I$(top_srcdir)/milena/doc/tutorial/tools

TUTORIAL_SRCDIR = $(top_srcdir)/milena/doc/tutorial
OUTPUTS_SRCDIR = $(TUTORIAL_SRCDIR)/outputs
FIGURES_SRCDIR = $(TUTORIAL_SRCDIR)/figures
SAMPLES_SRCDIR = $(TUTORIAL_SRCDIR)/samples

TUTORIAL_BUILDDIR = $(top_builddir)/milena/doc/tutorial
OUTPUTS_BUILDDIR = $(TUTORIAL_BUILDDIR)/outputs
SPLITTED_OUTPUTS_BUILDDIR = $(TUTORIAL_BUILDDIR)/outputs/splitted
FIGURES_BUILDDIR = $(TUTORIAL_BUILDDIR)/figures
SAMPLES_BUILDDIR = $(TUTORIAL_BUILDDIR)/samples


