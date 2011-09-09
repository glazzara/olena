# Copyright (C) 2006, 2007, 2008, 2009, 2010 EPITA Research and Development
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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = build-aux external milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

if ENABLE_SCRIBO
  SUBDIRS += scribo
endif ENABLE_SCRIBO

# Target shortcuts delegating the actual action to milena/Makefile.
LOCAL_RECURSIVE_TARGETS = pretty-check tests
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
  $(distdir)/milena/tests/value/stack.cc

PORTFILE=distrib/macports/Portfile

TARBALL_GZ=$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz
do_subst = sed -e 's|@packagename[@]|$(PACKAGE_TARNAME)|g'				\
	       -e 's|@version[@]|$(PACKAGE_VERSION)|g'					\
	       -e "s|@package_md5[@]|`md5sum $(TARBALL_GZ) | cut -d ' ' -f 1`|g" 	\
	       -e "s|@package_sha1[@]|`openssl sha1 $(TARBALL_GZ) | cut -d ' ' -f 2`|g"	\
	       -e "s|@package_rmd160[@]|`openssl rmd160 $(TARBALL_GZ) | cut -d ' ' -f 2`|g"

$(PORTFILE): dist $(top_srcdir)/distrib/macports/Portfile.in Makefile
	rm -f $@ $@.tmp
	rm -Rf distrib
	srcdir=''; \
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(mkdir_p) distrib/macports
	md5=`md5sum $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz`
	sha1=`openssl sha1 $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz`
	rmd160=`openssl rmd160 $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz`;
	$(do_subst) $(srcdir)/$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES = $(PORTFILE)


port: $(PORTFILE)

## FIXME: Running `build-aux/build_unit_test.sh' just to update
## `milena/tests/unit_test/unit-tests.mk' is overkill:
##
## 1. It recreates *all* the sources (more than 1200 `.cc' files!).
##    => We should just regen `unit-tests.mk', not the `.cc' files.
##
## 2. It visits (almost) all the directories and files under
##    milena/mln/ (which admittedly a smaller issue compared to the
##    previous one)
##    => Prune `unit-tests.mk' instead of regenerating it.  The
##       simplest solution is probably to equip `unit-tests.mk' in the
##       first place with `<<lrde'/`>>' tags, then prune it with
##       dist-strip.
##

# Do not distribute parts tagged `lrde' (see build-aux/strip-lrde).
dist-hook:
	for f in $(LRDE_STRIPPED_DIST_FILES); do	\
	  $(top_srcdir)/build-aux/strip-lrde $$f;	\
	done;						\
	rm -rf $(LRDE_NODIST_FILES) &&			\
	$(top_srcdir)/build-aux/build_unit_test.sh 	\
	    $(distdir)/milena/mln 			\
	    $(distdir)/milena/tests/unit_test 		\
	    $(top_srcdir)/milena/tests/unit_test/disabled_tests \
	    mln 					\
	&& 						\
	(						\
	  cd $(distdir);				\
	  set -evx;					\
	  autoreconf -f -v -i;				\
	  find . -name autom4te.cache | xargs rm -rf	\
	)


# Set the package version to olena-x.x-snapshot-dd-mm-yy.
#
# Do not check NEWS file (this is not a release so there is no
# entries in this file for this package).
snapshot:
	current_date=`date +'%d-%m-%y'` 				\
	&& sed -e "s/^\(AC_INIT(\[Olena\],\) \[\([\.[:alnum:]]*\)\(-snapshot-.*-[[:alnum:]]*\)*\], \(.*\)/\1 [\2-snapshot-$$current_date], \4/g" 	\
	       -e 's/ check-news / /g'					\
	       $(srcdir)/configure.ac > $(srcdir)/configure.ac.tmp 	\
	&& cp -f $(srcdir)/configure.ac $(srcdir)/configure.ac.old 	\
	&& cp -f $(srcdir)/configure.ac.tmp $(srcdir)/configure.ac 	\
	&& $(MAKE) $(AM_MAKEFLAGS) dist 				\
	&& mv -f $(srcdir)/configure.ac.old $(srcdir)/configure.ac
#>>
