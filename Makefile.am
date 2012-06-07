# Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 EPITA
# Research and Development Laboratory (LRDE).
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

SUBDIRS = build-aux milena

if ENABLE_SWILENA
  SUBDIRS += swilena
endif ENABLE_SWILENA

if ENABLE_SCRIBO
  SUBDIRS += scribo
endif ENABLE_SCRIBO

# We rely on Automake to ensure that the documentions are generated in
# the right order.  Here, we make sure that the general documentation
# will be generated after Milena's and Scribo's documentation.
SUBDIRS += doc

# Target shortcuts delegating the actual action to milena/Makefile.
LOCAL_RECURSIVE_TARGETS = tests
.PHONY: $(LOCAL_RECURSIVE_TARGETS)
$(LOCAL_RECURSIVE_TARGETS):
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@
	cd scribo && $(MAKE) $(AM_MAKEFLAGS) $@
.PHONY: pretty-check
pretty-check:
	cd milena && $(MAKE) $(AM_MAKEFLAGS) $@

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += milena scribo


TARBALL_GZ = $(distdir).tar.gz
do_subst =								       \
  sed -e 's|@packagename[@]|$(PACKAGE_TARNAME)|g'			       \
      -e 's|@version[@]|$(PACKAGE_VERSION)|g'				       \
      -e "s|@package_md5[@]|`md5sum $(TARBALL_GZ) | cut -d ' ' -f 1`|g"        \
      -e "s|@package_sha1[@]|`openssl sha1 $(TARBALL_GZ) | cut -d ' ' -f 2`|g" \
      -e "s|@package_rmd160[@]|`openssl rmd160 $(TARBALL_GZ) | cut -d ' ' -f 2`|g"

PORTFILE = distrib/macports/Portfile
$(PORTFILE): distrib/macports/Portfile.in Makefile dist
	rm -f $@ $@.tmp
	rm -Rf distrib
	srcdir=''; \
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(MKDIR_P) distrib/macports
	$(do_subst) $(srcdir)/$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@
CLEANFILES = $(PORTFILE)

.PHONY: port
port: $(PORTFILE)

deb:
	git-buildpackage --git-export-dir=../build-area/ \
			 --git-builder="debuild -i'\.git' -I.git "


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


# Extra files for debian package generation.
pixmapsdir = $(datarootdir)/pixmaps
dist_pixmaps_DATA = debian/olena.xpm

EXTRA_DIST = 			\
  debian/olena.install		\
  debian/olena-dev.doc-base	\
  debian/watch			\
  debian/gbp.conf		\
  debian/compat			\
  debian/README.Debian		\
  debian/rules			\
  debian/olena.manpages		\
  debian/changelog		\
  debian/olena-bin.menu		\
  debian/HOWTO			\
  debian/olena-bin.desktop	\
  debian/control		\
  debian/olena-dev.install	\
  debian/scribo-cli.1		\
  debian/olena-bin.install	\
  debian/olena-dev.docs		\
  debian/scribo-viewer.1	\
  debian/olena.1		\
  debian/copyright		\
  debian/source			\
  debian/source/local-options	\
  debian/source/options		\
  debian/source/format		\
  debian/source/lintian-overrides\
  debian/olena-bin.manpages
