# Copyright (C) 2011, 2013 EPITA Research and Development Laboratory
# (LRDE).
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


# Look for Scribo headers in srcdir and for (generated) test headers
# (like data.hh) in builddir.
AM_CPPFLAGS = -I$(top_srcdir)/scribo -I$(top_builddir)/scribo

DOXYGEN = doxygen

DOXYFILE = Doxyfile

DATA_html_dirs =

## --------------------- ##
## LaTeX configuration.  ##
## --------------------- ##

include $(top_srcdir)/build-aux/tex.mk

TEXI2DVI_FLAGS +=							\
  -I $(srcdir) -I $(srcdir)/outputs -I $(srcdir)/outputs/split		\
  -I $(srcdir)/img -I $(srcdir)/examples/split

# Additional files for LaTeX compilations.
DOC_PACKAGES = doxyfuns.sty scribo.sty


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
	   -e 's,@abs_top_srcdir\@,$(abs_top_srcdir),g'		\
	   -e 's,@top_builddir\@,$(top_builddir),g'		\
	   -e 's,@srcdir\@,$(srcdir),g'				\
	   -e 's,@builddir\@,$(builddir),g'

$(DOXYFILE_USER): $(srcdir)/$(DOXYFILE).in
	Id=`grep '^\$$Id' $(top_srcdir)/scribo/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	  $(edit) $< >$@
MAINTAINERCLEANFILES = $(DOXYFILE_USER)

USER_REFMAN = user-refman

# PDF.
USER_REFMAN_PDF = user-refman.pdf
dist_pdf_DATA = $(srcdir)/$(USER_REFMAN_PDF)

$(srcdir)/$(USER_REFMAN_PDF): $(srcdir)/$(USER_REFMAN)
	cd $(srcdir)/$(USER_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(USER_REFMAN)/latex/refman.pdf $@

# HTML.
EXTRA_DIST = $(srcdir)/user-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/user-refman.stamp
$(srcdir)/user-refman.stamp: $(srcdir)/$(DOXYFILE).in $(REFMAN_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(USER_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER)
	$(DOXYGEN) $(DOXYFILE_USER)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(USER_REFMAN).tmp/latex/refman.tex
	rm -rf $(srcdir)/$(USER_REFMAN)
## Fix path to the main page of the documentation.
	find $(USER_REFMAN).tmp -name '*.html' -exec			\
	  perl -pi -e "s,\@relcommondocpath\@,../../../../doc/,g;"	\
	           -e "s,\@userrefman\@,$(USER_REFMAN),g"		\
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


# ---------------------------- #
# Developer Reference Manual.  #
# ---------------------------- #

# Run "make regen-devel-doc" to forcefully rebuild the Doxygen
# developer documentation.
.PHONY: regen-devel-doc
regen-devel-doc:
	rm -f $(srcdir)/devel-refman.stamp
	$(MAKE) $(AM_MAKEFLAGS) $(srcdir)/devel-refman.stamp

DOXYFILE_DEVEL = Doxyfile_devel

edit_devel = sed							      \
  -e 's,OUTPUT_DIRECTORY       = @builddir@/user-refman.tmp,OUTPUT_DIRECTORY       = @builddir@/devel-refman.tmp,' \
  -e 's,EXTRACT_ALL            = NO,EXTRACT_ALL            = YES,'	      \
  -e 's,EXTRACT_PRIVATE        = NO,EXTRACT_PRIVATE        = YES,'	      \
  -e 's,EXTRACT_STATIC         = NO,EXTRACT_STATIC         = YES,'	      \
  -e 's,EXTRACT_LOCAL_CLASSES  = NO,EXTRACT_LOCAL_CLASSES  = YES,'	      \
  -e 's,HIDE_UNDOC_MEMBERS     = YES,HIDE_UNDOC_MEMBERS     = NO,'	      \
  -e 's,HIDE_UNDOC_CLASSES     = YES,HIDE_UNDOC_CLASSES     = NO,'	      \
  -e 's,HIDE_FRIEND_COMPOUNDS  = YES,HIDE_FRIEND_COMPOUNDS  = NO,'	      \
  -e 's,HIDE_IN_BODY_DOCS      = YES,HIDE_IN_BODY_DOCS      = NO,'	      \
  -e 's,INTERNAL_DOCS          = NO,INTERNAL_DOCS          = YES,'	      \
  -e 's,GENERATE_TODOLIST      = NO,GENERATE_TODOLIST      = YES,'	      \
  -e 's,PROJECT_NUMBER         = \"User documentation ,PROJECT_NUMBER         = \",'	 \
  -e 's,EXCLUDE_SYMBOLS	       = \*::internal\* \*_ mln::trait::\*,EXCLUDE_SYMBOLS	       =,'

$(DOXYFILE_DEVEL): $(srcdir)/$(DOXYFILE).in
	rm -f $@.tmp
	Id=`grep '^\$$Id' $(top_srcdir)/scribo/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	  $(edit) $< >$@.tmp
	$(edit_devel) $@.tmp >$@
	rm -f $@.tmp
MAINTAINERCLEANFILES += $(DOXYFILE_DEVEL)

DEVEL_REFMAN = devel-refman

# PDF.
DEVEL_REFMAN_PDF = devel-refman.pdf
# Be warned, most LaTeX installations will not be able to compile the
# PDF development documentation as-is, as TeX's default memory
# capacities are usually too small to handle such a large document.
$(srcdir)/$(DEVEL_REFMAN_PDF): $(srcdir)/$(DEVEL_REFMAN)
	cd $(srcdir)/$(DEVEL_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(DEVEL_REFMAN)/latex/refman.pdf $@

# HTML.
MAINTAINERCLEANFILES += $(srcdir)/devel-refman.stamp
$(srcdir)/devel-refman.stamp: $(srcdir)/$(DOXYFILE).in $(REFMAN_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(DEVEL_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_DEVEL)
	$(DOXYGEN) $(DOXYFILE_DEVEL)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(DEVEL_REFMAN).tmp/latex/refman.tex
	rm -rf $(srcdir)/$(DEVEL_REFMAN)
## Fix path to the main page of the documentation.
	perl -pi -e "s,\@relcommondocpath\@,../../../../doc/,g;"	\
	         -e "s,\@userrefman\@,$(DEVEL_REFMAN),g"		\
	  $(DEVEL_REFMAN).tmp/html/index.html
## FIXME: Moving directories between file systems is not portable.
	mv $(DEVEL_REFMAN).tmp $(srcdir)/$(DEVEL_REFMAN)
	@mv -f $@.tmp $@

$(srcdir)/$(DEVEL_REFMAN): $(srcdir)/devel-refman.stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

## -------------- ##
## Distribution.  ##
## -------------- ##

EXTRA_DIST +=					\
  $(DATA_html_dirs)				\
  $(DOXYFILE).in				\
  doxyfuns.sty					\
  scribo.sty					\
  tools/todoxygen.sh


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
	$(MKDIR_P) $(DESTDIR)$(htmldir)/scribo/
# The user reference manual is located in a subdirectory, hence this
# special case.
	rm -rf $(DESTDIR)$(htmldir)/scribo/$(USER_REFMAN)/html
	cp -r $(srcdir)/$(USER_REFMAN)/html \
		$(DESTDIR)$(htmldir)/scribo/$(USER_REFMAN)
## Fix path to the main page of the documentation.
	chmod -R u+w $(DESTDIR)$(htmldir)/scribo/$(USER_REFMAN)
	find $(DESTDIR)$(htmldir)/scribo/$(USER_REFMAN) -name '*.html' -exec  \
	  perl -pi -e "s,../../../../doc/$(USER_REFMAN)/html,../../$(USER_REFMAN),g;" \
	           -e "s,../../../../milena/doc/$(USER_REFMAN)/html,../../milena/$(USER_REFMAN)/,g" \
	    {} \;
# Other HTML directories can be processed uniformly.
	for d in $(DATA_html_dirs_inst); do			\
	  rm -rf $(DESTDIR)$(htmldir)/scribo/$$d;	\
	  cp -r $(srcdir)/$$d $(DESTDIR)$(htmldir)/scribo;\
	done
## Try GNU chmod's X mode before resorting to something slower but
## more standard.
	chmod -R a+rX $(DESTDIR)$(htmldir)/scribo ||	\
	  find $(DESTDIR)$(htmldir)/scribo		\
	       \( -type d -a -exec chmod a+rx {} \; \)	\
	       -o -exec chmod a+r {} \;

uninstall-local:
	for d in $(DATA_html_dirs_inst); do			\
	  chmod -Rf 700 $(DESTDIR)$(htmldir)/scribo/$$d;	\
	  rm -rf $(DESTDIR)$(htmldir)/scribo/$$d;		\
	done
# Same remark as above regarding the special case of the user
# reference manual.
	chmod -Rf 700 $(DESTDIR)$(htmldir)/scribo/
	rm -rf $(DESTDIR)$(htmldir)/scribo/$(USER_REFMAN)
	rm -rf $(DESTDIR)$(htmldir)/scribo


doc-teamcity:
	find  $(srcdir)/$(USER_REFMAN) -name '*.html' -exec		       \
	  perl -pi							       \
	    -e "s,../../../../milena/doc/$(USER_REFMAN)/html,../milena.doc,g;" \
	    -e "s,../../../../doc/$(USER_REFMAN)/html,../olena.doc,g"	       \
	    {} \;
