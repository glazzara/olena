# Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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
#

# FIXME: To be overhauled! (See ticket #134).

include $(top_srcdir)/milena/doc/doc.mk

SUBDIRS =		\
  examples		\
  ref_guide		\
  tutorial 		\
  white_paper


DOXYGEN = doxygen



.PHONY: doc						\
	doc-user doc-devel				\
	doc-pdf doc-html doc-devel-pdf doc-devel-html	\
	ref-doc-pdf ref-doc-html			\
	ref-doc-devel-pdf ref-doc-devel-html		\
	tutorial-pdf tutorial-html			\
	white-paper-pdf white-paper-html		\
	ref-guide-pdf ref-guide-html			\
	examples data-regen fig-convert


doc: doc-user doc-devel

doc-user: doc-pdf doc-html
doc-devel: doc-devel-pdf doc-devel-html

doc-pdf: tutorial-pdf white-paper-pdf ref-guide-pdf ref-doc-pdf

doc-html: tutorial-html white-paper-html ref-guide-html ref-doc-html 

doc-devel-pdf: tutorial-pdf ref-guide-pdf white-paper-pdf ref-doc-devel-pdf

doc-devel-html: tutorial-html ref-guide-html white-paper-html ref-doc-devel-html 

# Shared dependencies on the products of tutorial/ and ref_guide/.
EXTRA_DEPS = tutorial/tutorial.hh ref_guide/ref_guide.hh

# Doxygen PDF documentation outputs.
USER_REFMAN_PDF = user/latex/refman.pdf
ref-doc-pdf: $(USER_REFMAN_PDF)
$(USER_REFMAN_PDF): Doxyfile_user_pdf $(FIGURES_BUILDDIR) $(EXTRA_DEPS)
	$(DOXYGEN) Doxyfile_user_pdf
	cd user/latex && make pdf

USER_REFMAN_HTML = user/html
ref-doc-html: $(USER_REFMAN_HTML)
$(USER_REFMAN_HTML): Doxyfile_user $(FIGURES_BUILDDIR) $(EXTRA_DEPS)
	$(DOXYGEN) Doxyfile_user

# Doxygen HTML documentation output directories.
DEVEL_REFMAN_PDF = devel/latex/refman.pdf
ref-doc-devel-pdf: $(DEVEL_REFMAN_PDF)
$(DEVEL_REFMAN_PDF): Doxyfile_devel_pdf $(FIGURES_BUILDDIR) $(EXTRA_DEPS)
	$(DOXYGEN) Doxyfile_devel_pdf
	cd devel/latex && make $(AM_MAKEFLAGS)

DEVEL_REFMAN_HTML = devel/html
ref-doc-devel-html: $(DEVEL_REFMAN_HTML)
$(DEVEL_REFMAN_HTML): Doxyfile_devel $(FIGURES_BUILDDIR) $(EXTRA_DEPS)
	$(DOXYGEN) Doxyfile_devel

# Tutorial.
tutorial-pdf:
	cd tutorial && $(MAKE) $(AM_MAKEFLAGS) $@
tutorial-html:
	cd tutorial && $(MAKE) $(AM_MAKEFLAGS) $@

# White Paper.
white-paper-pdf:
	cd white_paper && $(MAKE) $(AM_MAKEFLAGS) $@

white-paper-html:
	cd white_paper && $(MAKE) $(AM_MAKEFLAGS) $@

# Reference Guide.
ref-guide-pdf:
	cd ref_guide && $(MAKE) $(AM_MAKEFLAGS) $@

ref-guide-html:
	cd ref_guide && $(MAKE) $(AM_MAKEFLAGS) $@


# FIXME: Adresse the construction of examples (see examples/Makefile.am).
data-regen:
	cd examples && $(MAKE) $(AM_MAKEFLAGS) $@


fix-refdata:
	cd examples && $(MAKE) $(AM_MAKEFLAGS) $@


fig-convert: $(srcdir)/figures.stamp

EXTRA_DIST = $(srcdir)/figures.stamp
$(srcdir)/figures.stamp: $(FIGURES_SRCDIR)/*.p*m
	rm -f $@.tmp
	touch $@.tmp
	test -d $(FIGURES_BUILDDIR) || mkdir $(FIGURES_BUILDDIR) &&	\
	failcom='exit 1';						\
	list="$(FIGURES_SRCDIR)/*.p*m"; for img in $$list; do		\
	  echo "Converting $$img to PNG";				\
	  name=`basename $$img`						\
	  ext=`echo $$name | cut -d '.' -f 2`;				\
	  convert -scale 250 $$img					\
	    $(FIGURES_BUILDDIR)/`basename $$img $$ext`png		\
	    || eval $$failcom;						\
	done
	mv -f $@.tmp $@


edit = sed -e "s|@ID@|$$Id|" 					\
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g' 		\
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g' 	\
	   -e 's,@top_builddir\@,$(top_builddir),g' 		\
	   -e 's,@top_srcdir\@,$(top_srcdir),g'

edit_pdf = sed -e 's,GENERATE_LATEX         = NO,GENERATE_LATEX = YES,g' \
	       -e 's,GENERATE_HTML          = YES,GENERATE_HTML = NO,g'

# FIXME: This is not good.  We should set these parameters for both
# documentation (devel and user) using @VARIABLES@.  Don't generate
# Doxyfile_user from Doxyfile_devel!  Both should be a product
# derived from a single source, Doxyfile.in.
edit_user = sed -e 's,OUTPUT_DIRECTORY       = ./devel/,OUTPUT_DIRECTORY       = ./user/,g' 		\
		-e 's,EXTRACT_ALL            = YES,EXTRACT_ALL            = NO,g'			\
		-e 's,EXTRACT_PRIVATE        = YES,EXTRACT_PRIVATE        = NO,g'			\
		-e 's,EXTRACT_STATIC         = YES,EXTRACT_STATIC         = NO,g'			\
		-e 's,EXTRACT_LOCAL_CLASSES  = YES,EXTRACT_LOCAL_CLASSES  = NO,g'			\
		-e 's,HIDE_UNDOC_MEMBERS     = NO,HIDE_UNDOC_MEMBERS      = YES,g'			\
		-e 's,HIDE_UNDOC_CLASSES     = NO,HIDE_UNDOC_CLASSES      = YES,g'			\
		-e 's,HIDE_FRIEND_COMPOUNDS  = NO,HIDE_FRIEND_COMPOUNDS   = YES,g'			\
		-e 's,HIDE_IN_BODY_DOCS      = NO,HIDE_IN_BODY_DOCS       = YES,g'			\
		-e 's,INTERNAL_DOCS          = YES,INTERNAL_DOCS          = NO,g'			\
		-e 's,GENERATE_TODOLIST      = YES,GENERATE_TODOLIST      = NO,g'			\
		-e 's,PROJECT_NUMBER         = \",PROJECT_NUMBER          = \"User documentation ,g'	\
		-e 's,EXCLUDE_SYMBOLS	       =,EXCLUDE_SYMBOLS	  = *::internal* *_ mln::trait::*,g'	


# Regen files.
EXTRA_DIST += generate_dist_files.sh headers.stamp
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	@rm -f $(srcdir)/headers.stamp.tmp
	@touch $(srcdir)/headers.stamp.tmp
	cd $(srcdir) && ./generate_dist_files.sh
	@mv -f $(srcdir)/headers.stamp.tmp $(srcdir)/headers.stamp

$(srcdir)/examples/examples.mk: $(srcdir)/headers.stamp
$(srcdir)/figures/figures.mk: $(srcdir)/headers.stamp
$(srcdir)/outputs/outputs.mk: $(srcdir)/headers.stamp


$(srcdir)/headers.stamp: $(srcdir)/generate_dist_files.sh
	@rm -f $@.tmp
	@touch $@.tmp
	cd $(srcdir) && ./generate_dist_files.sh
	@mv -f $@.tmp $@


include $(srcdir)/examples/examples.mk
include $(srcdir)/figures/figures.mk
include $(srcdir)/outputs/outputs.mk


EXTRA_DIST += 			\
  Doxyfile.in 			\
  groups 			\
  $(USER_REFMAN_HTML)		\
  $(USER_REFMAN_PDF)		\
  tools/clearbanner.sh 		\
  tools/sample_utils.hh 	\
  tools/split_sample.sh 	\
  tools/todoxygen.sh

CLEANFILES =					\
  Doxyfile_user					\
  Doxyfile_user_pdf				\
  Doxyfile_devel				\
  Doxyfile_devel_pdf				\
  devel						\
  user

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
# Moreover, this file is updated whenever ChangeLog is touched: using
# sed instead of configure saves us a lot of time.
Doxyfile_user: Doxyfile_devel $(top_srcdir)/milena/ChangeLog 
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;	\
	$(edit) $< >$@

Doxyfile_user_pdf: Doxyfile_user $(top_srcdir)/milena/ChangeLog 
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;	\
	$(edit_pdf) $< >$@

Doxyfile_devel: $(srcdir)/Doxyfile.in $(top_srcdir)/milena/ChangeLog 
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;	\
	$(edit) $< >$@

Doxyfile_devel_pdf: $Doxyfile_devel $(top_srcdir)/milena/ChangeLog 
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog`;	\
	$(edit_pdf) $< >$@
