# Copyright (C) 2007, 2008, 2009, 2010, 2013 EPITA Research and
# Development Laboratory (LRDE).
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


# Find Milena headers.
AM_CPPFLAGS = -I$(top_srcdir)/milena

DOXYGEN = doxygen

DOXYFILE = Doxyfile


## --------------------- ##
## LaTeX configuration.  ##
## --------------------- ##

include $(top_srcdir)/build-aux/tex.mk

TEXI2DVI_FLAGS +=							\
  -I $(srcdir) -I $(srcdir)/outputs -I $(srcdir)/outputs/split		\
  -I $(srcdir)/img -I $(srcdir)/examples/split


# ----------------------- #
# User Reference Manual.  #
# ----------------------- #

all-local: $(srcdir)/$(USER_REFMAN)

# Run "make regen-doc" to forcefully rebuild the Doxygen documentation.
.PHONY: regen-doc
regen-doc:
	rm -f $(srcdir)/user-refman.stamp
	$(MAKE) $(AM_MAKEFLAGS) $(srcdir)/user-refman.stamp

DOXYFILE_USER = Doxyfile_user


# Images
REFMAN_IMAGES =					\
	$(srcdir)/images/olena_layers.png 	\
	$(srcdir)/images/logo.jpg

EXTRA_DIST = 					\
	$(DOXYFILE).in				\
	$(REFMAN_IMAGES)			\
	$(srcdir)/images/olena_layers.svg

# Documentation content
REFMAN_deps = $(srcdir)/footer.html 	  	\
	      $(srcdir)/header.html 	  	\
	      $(srcdir)/doxygen.css 	  	\
	      $(srcdir)/mainpage.dox 	  	\
	      $(srcdir)/about.dox 		\
	      $(srcdir)/DoxygenLayout.xml 	\
	      $(srcdir)/modules_list.dox  	\
	      $(srcdir)/olena.bib	  	\
	      $(srcdir)/lrde.bib	  	\
	      $(srcdir)/doc.bib	  		\
	      $(srcdir)/contributors.dox  	\
	      $(srcdir)/bibliography.dox  	\
	      $(srcdir)/licensing.dox 	 	\
	      $(srcdir)/online_resources.dox	\
	      $(srcdir)/supported_platforms.dox	\
	      $(srcdir)/get_sources.dox		\
	      $(srcdir)/subdoc_footer.html	\
	      contributors.html			\
	      lrde_olena.html

	      $(REFMAN_IMAGES)

EXTRA_DIST += $(REFMAN_deps)

## FIXME: We used to depend on Subversion's $Id$ (extracted from
## ChangeLog) before.  We should depend on something less prone to
## change from now on.  The best is probably to use `git-version-gen'
## from Gnulib (as Bison does).  For the moment, set ID to an empty
## string.
ID =
# Use Make to generate Doxyfile instead of `configure', as advised by
# Autoconf's manual (see section ``Installation Directory
# Variables'').
edit = sed -e "s|@ID[@]|$(ID)|"					\
	   -e 's,@PACKAGE_NAME[@],$(PACKAGE_NAME),g'		\
	   -e 's,@PACKAGE_VERSION[@],$(PACKAGE_VERSION),g'	\
	   -e 's,@top_srcdir[@],$(top_srcdir),g'		\
	   -e 's,@abs_top_srcdir[@],$(abs_top_srcdir),g'	\
	   -e 's,@top_builddir[@],$(top_builddir),g'		\
	   -e 's,@srcdir[@],$(srcdir),g'			\
	   -e 's,@builddir[@],$(builddir),g'

$(DOXYFILE_USER): $(srcdir)/$(DOXYFILE).in
	$(edit) $< >$@
MAINTAINERCLEANFILES = $(DOXYFILE_USER)

USER_REFMAN = user-refman


# HTML.
EXTRA_DIST += $(srcdir)/user-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/user-refman.stamp
$(srcdir)/user-refman.stamp: $(srcdir)/$(DOXYFILE).in $(REFMAN_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(USER_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER)
	$(DOXYGEN) $(DOXYFILE_USER)
	rm -rf $(srcdir)/$(USER_REFMAN)
## Fix path to the main page of the documentation.
	find $(USER_REFMAN).tmp/html -name '*.html' -exec	\
	  perl -pi -e "s,@relcommondocpath[@],../../,g;"	\
	           -e "s,@userrefman[@],$(USER_REFMAN),g"	\
	    {} \;
## FIXME: Moving directories between file systems is not portable.
	mv $(USER_REFMAN).tmp $(srcdir)/$(USER_REFMAN)
	@mv -f $@.tmp $@

$(srcdir)/$(USER_REFMAN): $(srcdir)/user-refman.stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# Installing a directory located in the source dir is not supported by
# Automake by default, so we cannot simply add `$(USER_REFMAN)/html'
# to `dist_doc_DATA'.  The rule `install-data-local' (see below) used
# to work around this limitation is borrowed from the Vaucanson
# project.
EXTRA_DIST += $(USER_REFMAN)


## -------------- ##
## Contributors.  ##
## -------------- ##

contributors.html: $(top_srcdir)/AUTHORS
	@test -n "$(RST2HTML)"						\
	  || { echo "error: rst2html not found during configure" >&2;	\
	       exit 1; }
	$(RST2HTML) --input-encoding=UTF-8 --output-encoding=UTF-8	\
	  --stylesheet-path=$(srcdir)/doxygen.css --link-stylesheet	\
	  $< >$@


## -------------- ##
## Bibliography.  ##
## -------------- ##

# Warning: also generates lrde_olena.cite
lrde_olena.bib: $(srcdir)/lrde.bib
	@test -n "$(BIB2BIB)"						\
	  || { echo "error: bib2bib not found during configure" >&2;	\
	       exit 1; }
	$(BIB2BIB) -s plain -oc lrde_olena.cite -ob $@	\
	  -c 'project = "Image" or project = "Olena"'	\
	  -s year -r $<

# Warning: also generates lrde_olena_bib.html
lrde_olena.html: lrde_olena.bib
	@test -n "$(BIBTEX2HTML)"					 \
	  || { echo "error: bibtex2html not found during configure" >&2; \
	       exit 1; }
# Use the current directory for temporary files, as TeXlive (2012)
# prevents bibtex2html from running bibtex in a temporary directory on
# Mac OS X (see https://www.lri.fr/~filliatr/bibtex2html/).
	env TMPDIR=. $(BIBTEX2HTML) -s plain -nokeywords		\
	  -citefile lrde_olena.cite -noabstract -nofooter -labelname	\
	  -nodoc -linebreak -nf urllrde PDF $<


EXTRA_DIST += 				\
      lrde_olena.cite 			\
      lrde_olena.html			\
      lrde_olena.html	 		\
      lrde_olena.bib			\
      lrde_olena_bib.html

MAINTAINERCLEANFILES += lrde_olena.cite 	\
			lrde_olena.bib 		\
			lrde_olena_bib.html 	\
			lrde_olena.html

## ---------- ##
## Cleaning.  ##
## ---------- ##

maintainer-clean-local:
	-rm -rf $(USER_REFMAN).tmp $(srcdir)/$(USER_REFMAN)
	-for d in $(DATA_html_dirs); do		\
	  rm -rf $$d.tmp $(srcdir)/$$d;		\
	done


## -------------- ##
## Installation.  ##
## -------------- ##


# We install by hand, otherwise Automake produces "install .../srcdoc
# .../srcdoc", which installs our dir into the previous one, instead
# of replacing it.
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(htmldir)
# The user reference manual is located in a subdirectory, hence this
# special case.

#FIXME: Find a way to clear old files without removing freshly
#installed module docs.
#
#	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN)
	cp -r $(srcdir)/$(USER_REFMAN)/html $(DESTDIR)$(htmldir)/$(USER_REFMAN)
# Fix documentation paths
	chmod -R u+w $(DESTDIR)$(htmldir)/$(USER_REFMAN)
	find  $(DESTDIR)$(htmldir)/$(USER_REFMAN) -name '*.html' -exec \
	  perl -pi -e "s,../../../milena/doc/$(USER_REFMAN)/html,../milena/$(USER_REFMAN),g;" \
	           -e "s,../../../scribo/doc/$(USER_REFMAN)/html,../scribo/$(USER_REFMAN),g;" \
	           -e "s,../../$(USER_REFMAN)/html,../$(USER_REFMAN),g" \
	    {} \;
## Try GNU chmod's X mode before resorting to something slower but
## more standard.
	chmod -R a+rX $(DESTDIR)$(htmldir) || \
	find $(DESTDIR)$(htmldir) \( -type d -a -exec chmod a+rx {} \; \) \
             -o -exec chmod a+r {} \;

uninstall-local:
# Same remark as above regarding the special case of the user
# reference manual.
	chmod -R 700 $(DESTDIR)$(htmldir)/$(USER_REFMAN)
	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN)


doc-teamcity:
	find  $(srcdir)/$(USER_REFMAN) -name '*.html' -exec		       \
	  perl -pi							       \
	       -e "s,../../../milena/doc/$(USER_REFMAN)/html,../milena.doc,g;" \
	       -e "s,../../../scribo/doc/$(USER_REFMAN)/html,../scribo.doc,g;" \
	       -e "s,../../$(USER_REFMAN)/html,../olena.doc,g"		       \
	    {} \;
