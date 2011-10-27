# Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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


# Look for Milena headers in srcdir and for (generated) test headers
# (like data.hh) in builddir.
AM_CPPFLAGS = -I$(top_srcdir)/milena -I$(top_builddir)/milena

DOXYGEN = doxygen

DOXYFILE = Doxyfile


## --------------------- ##
## LaTeX configuration.  ##
## --------------------- ##

include $(top_srcdir)/build-aux/tex.mk

TEXI2DVI_FLAGS +=							\
  -I $(srcdir) -I $(srcdir)/outputs -I $(srcdir)/outputs/split		\
  -I $(srcdir)/img -I $(srcdir)/examples/split

# Additional files for LaTeX compilations.
DOC_PACKAGES = doxyfuns.sty milena.sty


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

REFMAN_deps = $(srcdir)/footer.html 	  	\
	      $(srcdir)/header.html 	  	\
	      $(srcdir)/doxygen.css 	  	\
	      $(srcdir)/mainpage.dox 	  	\
	      $(srcdir)/DoxygenLayout.xml 	\
	      $(srcdir)/modules_list.dox  	\
	      $(srcdir)/olena.bib	  	\
	      $(srcdir)/contributors.dox  	\
	      $(srcdir)/bibliography.dox  	\
	      $(srcdir)/license.dox 	 	\
	      $(srcdir)/license_commercial.dox 	\
	      contributors.html			\
	      lrde_olena.html

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
#
## FIXME: This is because, as in TC, we depend on $Id$ from the
## ChangeLog.  Maybe we should depend from something less prone to
## change.  See Bison's build system.
edit = sed -e "s|@ID@|$$Id|"					\
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g'		\
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g'	\
	   -e 's,@top_srcdir\@,$(top_srcdir),g'			\
	   -e 's,@top_builddir\@,$(top_builddir),g'		\
	   -e 's,@srcdir\@,$(srcdir),g'				\
	   -e 's,@builddir\@,$(builddir),g'

$(DOXYFILE_USER): $(srcdir)/$(DOXYFILE).in
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	  $(edit) $< >$@
MAINTAINERCLEANFILES = $(DOXYFILE_USER)

USER_REFMAN = user-refman


# HTML.
EXTRA_DIST = $(srcdir)/user-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/user-refman.stamp
$(srcdir)/user-refman.stamp: $(srcdir)/$(DOXYFILE).in $(REFMAN_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(USER_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER)
	$(DOXYGEN) $(DOXYFILE_USER)
	rm -rf $(srcdir)/$(USER_REFMAN)
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
	rst2html --stylesheet-path=$(srcdir)/doxygen.css --link-stylesheet \
		 --input-encoding=latin1 --output-encoding=latin1 \
		 $(top_srcdir)/AUTHORS > contributors.html


## -------------- ##
## Bibliography.  ##
## -------------- ##

# Warning: also generate lrde_olena.cite
lrde_olena.bib:
	bib2bib -s plain -oc lrde_olena.cite -ob lrde_olena.bib -c \
		'project = "Image" or project = "Olena"' \
		-s year -r $(srcdir)/lrde.bib

# Warning: also generate lrde_olena_bib.html
lrde_olena.html: lrde_olena.bib
	bibtex2html -s plain -nokeywords -citefile lrde_olena.cite \
		-noabstract -nofooter -labelname -nodoc -linebreak -nf urllrde PDF lrde_olena.bib


MAINTAINERCLEANFILES += lrde_olena.cite 	\
			lrde_olena.bib 		\
			lrde_olena_bib.html 	\
			lrde_olena.html

## ---------- ##
## Cleaning.  ##
## ---------- ##

MAINTAINERCLEANFILES += $(dist_pdf_DATA)

maintainer-clean-local:
	-rm -rf $(USER_REFMAN).tmp $(srcdir)/$(USER_REFMAN)
	-rm -rf $(DEVEL_REFMAN).tmp $(srcdir)/$(DEVEL_REFMAN)
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
	$(mkdir_p) $(DESTDIR)$(htmldir)
# The user reference manual is located in a subdirectory, hence this
# special case.
	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN)
	cp -r $(srcdir)/$(USER_REFMAN)/html $(DESTDIR)$(htmldir)/$(USER_REFMAN)
# Update paths to milena documentation directory
	$(DESTDIR)$(htmldir)/$(USER_REFMAN)/installdox \
		-l milena.tag@$(DESTDIR)$(htmldir)/$(USER_REFMAN)/milena \
		 `find $(DESTDIR)$(htmldir)/$(USER_REFMAN)/ -name '*.html'` \
		-l scribo.tag@$(DESTDIR)$(htmldir)/$(USER_REFMAN)/scribo \
		 `find $(DESTDIR)$(htmldir)/$(USER_REFMAN)/ -name '*.html'`
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
