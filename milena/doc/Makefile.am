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
#

# FIXME: To be overhauled! (See ticket #134).

include $(top_srcdir)/milena/doc/doc.mk

DOXYGEN = doxygen

DOXYFILE = Doxyfile

# Initialiaze variables.
EXTRA_DIST =
dist_doc_DATA =
CLEANFILES =

# FIXME: Simplify all of this.  ``Devel'' targets are really
# secondary.  And we probably don't need so much target aliases!

.PHONY: doc						\
	doc-user doc-devel				\
	doc-pdf doc-html doc-devel-pdf doc-devel-html	\
	ref-doc-pdf ref-doc-html			\
	ref-doc-devel-pdf ref-doc-devel-html

doc: doc-user doc-devel

doc-user: doc-pdf doc-html
doc-devel: doc-devel-pdf doc-devel-html

doc-pdf: tutorial-pdf white-paper-pdf ref-guide-pdf ref-doc-pdf

doc-html: tutorial-html white-paper-html ref-guide-html ref-doc-html

doc-devel-pdf: tutorial-pdf ref-guide-pdf white-paper-pdf ref-doc-devel-pdf

doc-devel-html: tutorial-html ref-guide-html white-paper-html \
		ref-doc-devel-html


## -------------------------------------------------- ##
## Shared dependencies on the products of tutorial/.  ##
## -------------------------------------------------- ##

# FIXME: Maintaining dependencies across directories is too
# complicated.  Simplify.
EXTRA_DEPS = $(srcdir)/tutorial/tutorial.hh $(srcdir)/ref_guide/ref_guide.hh

## ----------------------- ##
## Doxygen documentation.  ##
## ----------------------- ##

## FIXME: We should use a stamp file and generate the documentation in
## a temporary directory in the _build_ tree, then copy it into a
## directory of the _source_ tree.  See how this is implemented in
## Vaucanson's doc build system.  (Do not forget to adjust Doxyfiles.)

# ----------------------------- #
# User reference manual (PDF).  #
# ----------------------------- #

# FIXME: Keep it or throw it away?
all-local: $(srcdir)/$(USER_REFMAN_PDF)

DOXYFILE_USER_PDF = Doxyfile_user_pdf

## FIXME: Rename USER_REFMAN_PDF to something else?
USER_REFMAN_PDF = user-refman.pdf
## FIXME: Likewise?
USER_REFMAN_LATEX = user-refman-latex

ref-doc-pdf: $(srcdir)/$(USER_REFMAN_PDF)

# FIXME: Use texi2dvi instead of Doxygen's generated Makefile?
# (The LaTeX to PDF rule could be factored using a suffix rule, as in
# LRDE's share/).
$(srcdir)/$(USER_REFMAN_PDF): $(srcdir)/$(USER_REFMAN_LATEX)
	cd $(srcdir)/$(USER_REFMAN_LATEX) && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(USER_REFMAN_LATEX)/refman.pdf $@

## FIXME: Use a variable instead of `$(srcdir)/user/latex'.
##
## FIXME: Are dependencies $(srcdir)/figures.stamp $(EXTRA_DEPS) set
## on the right rule?  Does Doxygen make a copy of figures, or does it
## generate LaTeX inputs relying on the existence of such figures in
## the initial location?  Investigate.
$(srcdir)/user-refman-latex.stamp: $(srcdir)/$(DOXYFILE).in $(srcdir)/figures.stamp $(EXTRA_DEPS)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(srcdir)/user/latex
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER_PDF)
	$(DOXYGEN) $(DOXYFILE_USER_PDF)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(srcdir)/user/latex/refman.tex
	-rm -rf $(srcdir)/$(USER_REFMAN_LATEX)
## FIXME: moving directories between file systems is not portable.
	mv $(srcdir)/user/latex $(srcdir)/$(USER_REFMAN_LATEX)
	@mv -f $@.tmp $@

# FIXME: Probably superfluous.  Should vanish when HTML and LaTeX
# genarations are merged.
$(srcdir)/$(USER_REFMAN_LATEX): $(srcdir)/user-refman-latex.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# Clean Doxygen products.
.PHONY: clean-user-refman-latex
clean-user-refman-latex:
	rm -rf $(srcdir)/user/latex $(srcdir)/$(USER_REFMAN_LATEX)

# ------------------------------ #
# User reference manual (HTML).  #
# ------------------------------ #

# FIXME: Keep it or throw it away?
all-local: $(srcdir)/user-refman-html.stamp

# FIXME: Rename USER_REFMAN_HTML as DOCDIR_USER_HTML?
USER_REFMAN_HTML = user-refman-html

DOXYFILE_USER_HTML = Doxyfile_user_html

ref-doc-html: $(srcdir)/$(USER_REFMAN_HTML)

# FIXME: Use `$(USER_REFMAN_HTML).tmp' instead of `$(srcdir)/user/html'.
$(srcdir)/user-refman-html.stamp: $(srcdir)/$(DOXYFILE).in $(srcdir)/figures.stamp $(EXTRA_DEPS)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(srcdir)/user/html
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER_HTML)
	$(DOXYGEN) $(DOXYFILE_USER_HTML)
	-rm -rf $(srcdir)/$(USER_REFMAN_HTML)
## FIXME: moving directories between file systems is not portable.
	mv $(srcdir)/user/html $(srcdir)/$(USER_REFMAN_HTML)
	@mv -f $@.tmp $@

$(srcdir)/$(USER_REFMAN_HTML): $(srcdir)/user-refman-html.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# Clean Doxygen products.
.PHONY: clean-user-refman-html
clean-user-refman-html:
	rm -rf $(srcdir)/user/html $(srcdir)/$(USER_REFMAN_HTML)

# ---------------------------------- #
# Developer reference manual (PDF).  #
# ---------------------------------- #

DOXYFILE_DEVEL_PDF = Doxyfile_devel_pdf

## FIXME: Rename DEVEL_REFMAN_PDF to something else?
DEVEL_REFMAN_PDF = devel-refman-pdf
## FIXME: Likewise?
DEVEL_REFMAN_LATEX = devel-refman-latex

ref-doc-devel-pdf: $(srcdir)/$(DEVEL_REFMAN_PDF)

## FIXME: Use texi2dvi instead of Doxygen's generated Makefile?  (The
## LaTeX to PDF rule could be factored using a suffix rule, as in
## LRDE's share/).
$(srcdir)/$(DEVEL_REFMAN_PDF): $(srcdir)/$(DEVEL_REFMAN_LATEX)
	cd $(srcdir)/$(DEVEL_REFMAN_LATEX) && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(DEVEL_REFMAN_LATEX)/refman.pdf $@

## FIXME: Use a variable instead of `$(srcdir)/devel/latex'.
##
## FIXME: Are dependencies $(srcdir)/figures.stamp $(EXTRA_DEPS) set
## on the right rule?  Does Doxygen make a copy of figures, or does it
## generate LaTeX inputs relying on the existence of such figures in
## the initial location?  Investigate.
$(srcdir)/devel-refman-latex.stamp: $(srcdir)/$(DOXYFILE).in $(srcdir)/figures.stamp $(EXTRA_DEPS)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(srcdir)/devel/latex
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_DEVEL_PDF)
	$(DOXYGEN) $(DOXYFILE_DEVEL_PDF)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(srcdir)/devel/latex/refman.tex
	-rm -rf $(srcdir)/$(DEVEL_REFMAN_LATEX)
## FIXME: moving directories between file systems is not portable.
	mv $(srcdir)/devel/latex $(srcdir)/$(DEVEL_REFMAN_LATEX)
	@mv -f $@.tmp $@

# FIXME: Probably superfluous.  Should vanish when HTML and LaTeX
# genarations are merged.
$(srcdir)/$(DEVEL_REFMAN_LATEX): $(srcdir)/devel-refman-latex.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# Clean Doxygen products.
.PHONY: clean-devel-refman-latex
clean-devel-refman-latex:
	rm -rf $(srcdir)/devel/latex $(srcdir)/$(DEVEL_REFMAN_LATEX)

# ----------------------------------- #
# Developer reference manual (HTML).  #
# ----------------------------------- #

# FIXME: Rename DEVEL_REFMAN_HTML as DOCDIR_DEVEL_HTML?
DEVEL_REFMAN_HTML = devel-refman-html

DOXYFILE_DEVEL_HTML = Doxyfile_devel_html

ref-doc-devel-html: $(srcdir)/$(DEVEL_REFMAN_HTML)

# FIXME: Use `$(DEVEL_REFMAN_HTML).tmp' instead of `$(srcdir)/devel/html'.
$(srcdir)/devel-refman-html.stamp: $(srcdir)/$(DOXYFILE).in $(srcdir)/figures.stamp $(EXTRA_DEPS)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(srcdir)/devel/html
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_DEVEL_HTML)
	$(DOXYGEN) $(DOXYFILE_DEVEL_HTML)
	-rm -rf $(srcdir)/$(DEVEL_REFMAN_HTML)
## FIXME: moving directories between file systems is not portable.
	mv $(srcdir)/devel/html $(srcdir)/$(DEVEL_REFMAN_HTML)
	@mv -f $@.tmp $@

$(srcdir)/$(DEVEL_REFMAN_HTML): $(srcdir)/devel-refman-html.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# Clean Doxygen products.
.PHONY: clean-devel-refman-html
clean-devel-refman-html:
	rm -rf $(srcdir)/devel/html $(srcdir)/$(DEVEL_REFMAN_HTML)

# ---------- #
# Cleaning.  #
# ---------- #

maintainer-clean-local: clean-user-refman-latex	clean-user-refman-html
maintainer-clean-local: clean-devel-refman-latex clean-devel-refman-html
maintainer-clean-local:
	rm -rf $(srcdir)/user
	rm -rf $(srcdir)/devel

## ------------------------- ##
## Technical Documentation.  ##
## ------------------------- ##

include $(srcdir)/technical/technical.mk

## ---------- ##
## Tutorial.  ##
## ---------- ##

include $(srcdir)/tutorial/tutorial.mk

## ------------- ##
## White Paper.  ##
## ------------- ##

include $(srcdir)/white_paper/white_paper.mk

## ----------------- ##
## Reference Guide.  ##
## ----------------- ##

include $(srcdir)/ref_guide/ref_guide.mk



# Make sure the figures are the first files in EXTRA_DIST.
# Make the 'fake-doc' rule works correctly.
EXTRA_DIST +=
$(srcdir)/figures/figures.mk: $(srcdir)/headers.stamp
include $(srcdir)/figures/figures.mk


fig-convert: $(srcdir)/figures.stamp

EXTRA_DIST += figures.stamp
$(srcdir)/figures.stamp: $(FIGURES_SRCDIR)/*.p*m
	rm -f $@.tmp
	touch $@.tmp
	failcom='exit 1';						\
	list="$(FIGURES_SRCDIR)/*.p*m"; for img in $$list; do		\
	  echo "Converting $$img to PNG";				\
	  name=`basename $$img`						\
	  ext=`echo $$name | cut -d '.' -f 2`;				\
	  convert -scale 250 -bordercolor black -border 2x2 $$img 	\
	    $(FIGURES_SRCDIR)/`basename $$img $$ext`png			\
	    || eval $$failcom;						\
	done
	mv -f $@.tmp $@


edit = sed -e "s|@ID@|$$Id|"					\
	   -e 's,@PACKAGE_NAME\@,$(PACKAGE_NAME),g'		\
	   -e 's,@PACKAGE_VERSION\@,$(PACKAGE_VERSION),g'	\
	   -e 's,@top_srcdir\@,$(top_srcdir),g'			\
	   -e 's,@top_builddir\@,$(top_builddir),g'		\
	   -e 's,@srcdir\@,$(srcdir),g'				\
	   -e 's,@builddir\@,$(builddir),g'

## FIXME: Instead of generating HTML and PDF (LaTeX actualy) output in
## two passes, do it in a row.  It will save us a lot of time.
edit_pdf = sed -e 's,GENERATE_LATEX         = NO,GENERATE_LATEX = YES,g' \
	       -e 's,GENERATE_HTML          = YES,GENERATE_HTML = NO,g'

# FIXME: This is not good.  We should set these parameters for both
# documentation (devel and user) using @VARIABLES@.  Don't generate
# Doxyfile_user_html from Doxyfile_devel_html!  Both should be a product
# derived from a single source, Doxyfile.in.
edit_user = sed								      \
  -e 's,OUTPUT_DIRECTORY       = @srcdir@/devel/,OUTPUT_DIRECTORY       = @srcdir@/user/,g' \
  -e 's,EXTRACT_ALL            = YES,EXTRACT_ALL            = NO,g'	      \
  -e 's,EXTRACT_PRIVATE        = YES,EXTRACT_PRIVATE        = NO,g'	      \
  -e 's,EXTRACT_STATIC         = YES,EXTRACT_STATIC         = NO,g'	      \
  -e 's,EXTRACT_LOCAL_CLASSES  = YES,EXTRACT_LOCAL_CLASSES  = NO,g'	      \
  -e 's,HIDE_UNDOC_MEMBERS     = NO,HIDE_UNDOC_MEMBERS      = YES,g'	      \
  -e 's,HIDE_UNDOC_CLASSES     = NO,HIDE_UNDOC_CLASSES      = YES,g'	      \
  -e 's,HIDE_FRIEND_COMPOUNDS  = NO,HIDE_FRIEND_COMPOUNDS   = YES,g'	      \
  -e 's,HIDE_IN_BODY_DOCS      = NO,HIDE_IN_BODY_DOCS       = YES,g'	      \
  -e 's,INTERNAL_DOCS          = YES,INTERNAL_DOCS          = NO,g'	      \
  -e 's,GENERATE_TODOLIST      = YES,GENERATE_TODOLIST      = NO,g'	      \
  -e 's,PROJECT_NUMBER         = \",PROJECT_NUMBER          = \"User documentation ,g'	 \
  -e 's,EXCLUDE_SYMBOLS	       =,EXCLUDE_SYMBOLS	  = *::internal* *_ mln::trait::*,g'


## ---------- ##
## Examples.  ##
## ---------- ##

include $(top_srcdir)/milena/doc/examples/make.mk

# Regen files.
EXTRA_DIST += generate_dist_files.sh headers.stamp
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	@rm -f $(srcdir)/headers.stamp.tmp
	@touch $(srcdir)/headers.stamp.tmp
	@cd $(srcdir) && 				\
	  ./generate_dist_files.sh figures 'p*m' && 	\
	  ./generate_dist_files.sh outputs txt &&	\
	  ./generate_dist_files.sh examples cc.raw
	@mv -f $(srcdir)/headers.stamp.tmp $(srcdir)/headers.stamp

$(srcdir)/examples/examples.mk: $(srcdir)/headers.stamp
$(srcdir)/outputs/outputs.mk: $(srcdir)/headers.stamp


$(srcdir)/headers.stamp: $(srcdir)/generate_dist_files.sh
	@rm -f $@.tmp
	@touch $@.tmp
	@cd $(srcdir) && 				\
	  ./generate_dist_files.sh figures 'p*m' && 	\
	  ./generate_dist_files.sh outputs txt &&	\
	  ./generate_dist_files.sh examples cc.raw
	@mv -f $@.tmp $@


include $(srcdir)/examples/examples.mk
include $(srcdir)/outputs/outputs.mk


dist_doc_DATA += $(USER_REFMAN_PDF)

# Installing a directory located in the source dir is not supported by
# Automake by default, so we cannot add `USER_REFMAN_HTML' to
# `dist_doc_DATA'.  The following two rules are are workaround from
# the Vaucanson project.
EXTRA_DIST += $(USER_REFMAN_HTML)

# We install by hand, otherwise Automake produces "install .../srcdoc
# .../srcdoc", which installs our dir into the previous one, instead
# of replacing it.
install-data-local:
	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN_HTML)
	$(mkdir_p) $(DESTDIR)$(htmldir)
	cp -r $(srcdir)/$(USER_REFMAN_HTML) $(DESTDIR)$(htmldir)
## Try GNU chmod's X mode before resorting to something slower but
## more standard.
	chmod -R a+rX $(DESTDIR)$(htmldir) || \
	find $(DESTDIR)$(htmldir) \( -type d -a -exec chmod a+rx {} \; \) \
             -o -exec chmod a+r {} \;

uninstall-local:
	chmod -R 700 $(DESTDIR)$(htmldir)/$(USER_REFMAN_HTML)
	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN_HTML)

## FIXME: These are now generated in the build directory.
# EXTRA_DIST +=					\
#   Doxyfile_devel_html				\
#   Doxyfile_devel_pdf				\
#   Doxyfile_user_html				\
#   Doxyfile_user_pdf

EXTRA_DIST +=					\
  $(DOXYFILE).in					\
  groups/accu.hh				\
  groups/graph.hh				\
  groups/images.hh				\
  groups/main.hh				\
  groups/neighb.hh				\
  groups/site_set.hh				\
  groups/util.hh				\
  groups/win.hh					\
  img/small-enlarged.png			\
  img/small.png					\
  doxyfuns.sty					\
  milena.sty					\
  tools/clearbanner.sh				\
  tools/sample_utils.hh				\
  tools/split_sample.sh				\
  tools/todoxygen.sh

## FIXME: Unsure about this.  If the directory of the generated
## documentation is to be removed during `maintainer-clean' (see
## Vaucanson's doc/Makefile.am), then the source Doxyfiles should not
## be removed earlier (i.e. at `clean') !
##
## Note: if configure is used to generate Doxyfiles (see below), these
## files should be (and will be automatically) remove by `distclean'.
CLEANFILES +=					\
  Doxyfile_user					\
  Doxyfile_user_pdf				\
  Doxyfile_devel				\
  Doxyfile_devel_pdf				\
  devel						\
  user

# Sed is used to generate Doxyfile from Doxyfile.in instead of
# configure, because the former is way faster than the latter.
#
## FIXME: This is because, as in TC, we depend on $Id$ from the
## ChangeLog.  Maybe we should depend from something less prone to
## change.
##
## FIXME: Move these rules higher, closer to their use sites.
##
## FIXME: There are too many indirect generations here.  Simplify.

# FIXME: Directly depend on $(DOXYFILE).in instead.
$(DOXYFILE_USER_PDF): $(DOXYFILE_USER_HTML)
	$(edit_pdf) $< >$@

# FIXME: Directly depend on $(DOXYFILE).in instead.
$(DOXYFILE_USER_HTML): $(DOXYFILE_DEVEL_HTML)
	$(edit_user) $< >$@

# FIXME: Directly depend on $(DOXYFILE).in instead.
$(DOXYFILE_DEVEL_PDF): $(DOXYFILE_DEVEL_HTML)
	$(edit_pdf) $< >$@

$(DOXYFILE_DEVEL_HTML): $(srcdir)/$(DOXYFILE).in
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	$(edit) $< >$@
