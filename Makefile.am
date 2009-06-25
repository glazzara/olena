# Copyright (C) 2006, 2007, 2008, 2009 EPITA Research and Development
# Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

## Process this file through Automake to produce Makefile.in.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

#<<lrde
if ENABLE_SCRIBO
  SUBDIRS += scribo
endif ENABLE_SCRIBO
#>>

# Target shortcuts delegating the actual action to milena/Makefile.
LOCAL_RECURSIVE_TARGETS = doc pretty-check tests
$(LOCAL_RECURSIVE_TARGETS):
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: $(LOCAL_RECURSIVE_TARGETS)

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += milena scribo


#<<lrde
## FIXME: All of this it too much ad hoc.  Use Git branches to
## distinguish distributed files from non distributed ones after the
## conversion of the repository.

LRDE_STRIPPED_DIST_FILES =				\
  $(distdir)/Makefile.am				\
  $(distdir)/configure.ac				\
  $(distdir)/milena/doc/Makefile.am			\
  $(distdir)/milena/doc/ref_guide/Makefile.am		\
  $(distdir)/milena/doc/tutorial/Makefile.am		\
  $(distdir)/milena/doc/white_paper/Makefile.am		\
  $(distdir)/milena/mln/accu/math/all.hh		\
  $(distdir)/milena/mln/all.hh				\
  $(distdir)/milena/mln/core/image/vmorph/all.hh	\
  $(distdir)/milena/mln/core/image/vmorph/fun_image.hh	\
  $(distdir)/milena/mln/data/all.hh			\
  $(distdir)/milena/mln/essential/routine.hh		\
  $(distdir)/milena/mln/fun/all.hh			\
  $(distdir)/milena/mln/fun/essential.hh		\
  $(distdir)/milena/mln/fun/v2v/all.hh			\
  $(distdir)/milena/mln/fun/v2v/essential.hh		\
  $(distdir)/milena/mln/linear/all.hh			\
  $(distdir)/milena/mln/trait/all.hh			\
  $(distdir)/milena/mln/trait/ch_value.hh		\
  $(distdir)/milena/mln/value/all.hh			\
  $(distdir)/milena/mln/value/essential.hh		\
  $(distdir)/milena/tests/Makefile.am			\
  $(distdir)/milena/tests/convert/Makefile.am		\
  $(distdir)/milena/tests/extract/Makefile.am		\
  $(distdir)/milena/tests/fun/v2v/Makefile.am		\
  $(distdir)/milena/tests/linear/Makefile.am		\
  $(distdir)/milena/tests/linear/gaussian/Makefile.am	\
  $(distdir)/milena/tests/registration/Makefile.am	\
  $(distdir)/milena/tests/trait/ch_value.cc		\
  $(distdir)/milena/tests/value/Makefile.am

LRDE_NODIST_FILES =					\
  $(distdir)/milena/tests/convert/to_hsl.cc		\
  $(distdir)/milena/tests/extract/blue.cc		\
  $(distdir)/milena/tests/extract/green.cc		\
  $(distdir)/milena/tests/extract/hue.cc		\
  $(distdir)/milena/tests/extract/lum.cc		\
  $(distdir)/milena/tests/extract/red.cc		\
  $(distdir)/milena/tests/extract/sat.cc		\
  $(distdir)/milena/tests/fun/v2v/component.cc		\
  $(distdir)/milena/tests/fun/v2v/hsl_to_rgb.cc		\
  $(distdir)/milena/tests/fun/v2v/rgb_to_hsl.cc		\
  $(distdir)/milena/tests/linear/gaussian.cc		\
  $(distdir)/milena/tests/linear/gaussian/filter.cc	\
  $(distdir)/milena/tests/registration/registration.cc	\
  $(distdir)/milena/tests/value/stack.cc		\
  $(distdir)/scribo

# Do not distribute parts tagged `lrde' (see build-aux/strip-lrde).
dist-hook:
	for f in $(LRDE_STRIPPED_DIST_FILES); do	\
	  $(top_srcdir)/build-aux/strip-lrde $$f;	\
	done;						\
	rm -rf $(LRDE_NODIST_FILES) &&			\
	$(top_srcdir)/build-aux/build_unit_test.sh 	\
	    $(distdir)/milena/mln 			\
	    $(distdir)/milena/tests/unit_test 		\
	    $(top_srcdir)/milena/tests/unit_test/disabled_tests && \
	(						\
	  cd $(distdir);				\
	  set -evx;					\
	  autoreconf -f -v -i;				\
	  find . -name autom4te.cache | xargs rm -rf	\
	)
#>>
