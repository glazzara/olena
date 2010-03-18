## Figures' dependencies.			-*- Makefile-Automake -*-

## Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

## Figures depend on the timestamp associated with their generator.
## See also examples-outputs.mk.

## FIXME: Generate this file.
##
## It'd be cool if programs themselves could generate a list of their
## outputs (as a side effect) or at least if the extraction of this
## list from the source file was straightforward.

EXTEND_FIGURES =				\
  $(srcdir)/figures/extend-1.ppm		\
  $(srcdir)/figures/extend-2.ppm		\
  $(srcdir)/figures/extend-3.ppm		\
  $(srcdir)/figures/extend-4.ppm		\
  $(srcdir)/figures/extend-5.ppm
$(EXTEND_FIGURES): $(srcdir)/examples/extend.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

FILL_FIGURES =					\
  $(srcdir)/figures/fill-1.ppm			\
  $(srcdir)/figures/fill-2.ppm
$(FILL_FIGURES): $(srcdir)/examples/fill.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

FILL_SUBDOMAIN_FIGURES =			\
  $(srcdir)/figures/fill-subdomain-1.pbm	\
  $(srcdir)/figures/fill-subdomain-2.ppm	\
  $(srcdir)/figures/fill-subdomain-3.ppm	\
  $(srcdir)/figures/fill-subdomain-4.ppm
$(FILL_SUBDOMAIN_FIGURES): $(srcdir)/examples/fill-subdomain.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

FILL_IMAGEIF_CFUN_FIGURES = $(srcdir)/figures/fill-imageif-cfun-1.ppm
$(FILL_IMAGEIF_CFUN_FIGURES): $(srcdir)/examples/fill-imageif-cfun.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

IMA2D_ROT_FIGURES =				\
  $(srcdir)/figures/ima2d-rot-1.ppm		\
  $(srcdir)/figures/ima2d-rot-2.ppm
$(IMA2D_ROT_FIGURES): $(srcdir)/examples/ima2d-rot.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

IMA_SAVE_FIGURES = $(srcdir)/figures/ima_save.pbm
$(IMA_SAVE_FIGURES): $(srcdir)/examples/ima-save.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

LABELING_COMPUTE_FIGURES =			\
  $(srcdir)/figures/labeling-compute-1.pbm	\
  $(srcdir)/figures/labeling-compute-2.ppm
$(LABELING_COMPUTE_FIGURES): $(srcdir)/examples/labeling-compute.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

LOGICAL_NOT_FIGURES =				\
  $(srcdir)/figures/logical-not-1.pbm		\
  $(srcdir)/figures/logical-not-2.pbm		\
  $(srcdir)/figures/logical-not-3.pbm
$(LOGICAL_NOT_FIGURES): $(srcdir)/examples/logical-not.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

TUTO2_FIRST_IMAGE_FIGURES = $(srcdir)/figures/tuto2_first_image-1.pbm
$(TUTO2_FIRST_IMAGE_FIGURES): $(srcdir)/examples/tuto2_first_image.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

TUTO3_COLORIZE_FIGURES =			\
  $(srcdir)/figures/tuto3_colorize-1.pgm	\
  $(srcdir)/figures/tuto3_colorize-2.ppm
$(TUTO3_COLORIZE_FIGURES): $(srcdir)/examples/tuto3/colorize.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

TUTO3_RW_IMAGE_FIGURES =			\
  $(srcdir)/figures/tuto3_rw_image-1.ppm	\
  $(srcdir)/figures/tuto3_rw_image-3.ppm
$(TUTO3_RW_IMAGE_FIGURES): $(srcdir)/examples/tuto3_rw_image.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

TUTO4_GENERICITY_AND_ALGORITHMS_FIGURES =			\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-1.ppm	\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-2.ppm	\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-3.pbm	\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-4.ppm	\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-5.ppm	\
  $(srcdir)/figures/tuto4_genericity_and_algorithms-6.ppm
$(TUTO4_GENERICITY_AND_ALGORITHMS_FIGURES): $(srcdir)/examples/tuto4_genericity_and_algorithms.stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# The list of all generated figures.
FIGURES =					\
  $(EXTEND_FIGURES)				\
  $(FILL_FIGURES)				\
  $(FILL_SUBDOMAIN_FIGURES)			\
  $(FILL_IMAGEIF_CFUN_FIGURES)			\
  $(IMA2D_ROT_FIGURES)				\
  $(IMA_SAVE_FIGURES)				\
  $(LABELING_COMPUTE_FIGURES)			\
  $(LOGICAL_NOT_FIGURES)			\
  $(TUTO2_FIRST_IMAGE_FIGURES)			\
  $(TUTO3_COLORIZE_FIGURES)			\
  $(TUTO3_RW_IMAGE_FIGURES)			\
  $(TUTO4_GENERICITY_AND_ALGORITHMS_FIGURES)
