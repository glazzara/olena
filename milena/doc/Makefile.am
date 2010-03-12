
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

# Look for Milena header in srcdir and for (generated) test headers
# (like data.hh) in builddir.
AM_CPPFLAGS =						\
  -I$(top_srcdir)/milena -I$(top_builddir)/milena	\
  -I$(top_srcdir)/milena/doc/tutorial/tools

## FIXME: Upercase variables should be reserved for special names:
## classical Make variables (e.g. `CXXFLAGS') Automake variables
## (e.g. `SUBDIRS'), etc.
##
## Moreover, do we really need all these variables?  See how things
## evolve during the refurbishing of the build system in doc/.
DOC_SRCDIR = $(top_srcdir)/milena/doc
OUTPUTS_SRCDIR = $(DOC_SRCDIR)/outputs
SPLIT_OUTPUTS_SRCDIR = $(OUTPUTS_SRCDIR)/split
FIGURES_SRCDIR = $(DOC_SRCDIR)/figures
EXAMPLES_SRCDIR = $(DOC_SRCDIR)/examples
SPLIT_EXAMPLES_SRCDIR = $(EXAMPLES_SRCDIR)/split
IMG_SRCDIR = $(DOC_SRCDIR)/img

# FIXME: Adjust when $(DOC_SRCDIR) is renamed or removed.
#
# FIXME: Careful, `doc_dir' is close to `docdir', which is reserved by
# Automake.  Change this name?
doc_dir = $(DOC_SRCDIR)

DOXYGEN = doxygen

DOXYFILE = Doxyfile

# Initialiaze variables.
EXTRA_DIST =
dist_doc_DATA =
CLEANFILES =
MAINTAINERCLEANFILES =

# FIXME: Simplify all of this.  ``Devel'' targets are really
# secondary.  And we probably don't need so much target aliases!

.PHONY: doc						\
	doc-user doc-devel				\
	doc-pdf doc-html doc-devel-pdf doc-devel-html	\
	ref-doc ref-doc-pdf				\
	ref-doc-devel ref-doc-devel-pdf

doc: doc-user doc-devel

doc-user: doc-pdf doc-html
doc-devel: doc-devel-pdf doc-devel-html

doc-pdf: tutorial-pdf white-paper-pdf ref-guide-pdf ref-doc-pdf

doc-html: tutorial-html white-paper-html ref-guide-html ref-doc

doc-devel-pdf: tutorial-pdf ref-guide-pdf white-paper-pdf ref-doc-devel-pdf

doc-devel-html: tutorial-html ref-guide-html white-paper-html \
		ref-doc-devel


## ----------------------- ##
## Doxygen documentation.  ##
## ----------------------- ##

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    corresponding to a bunch of files.
refman_dependencies =				\
  $(srcdir)/split-examples.stamp		\
  $(srcdir)/split-outputs.stamp			\
  $(srcdir)/figures.stamp			\
  $(srcdir)/tutorial/tutorial.hh		\
  $(srcdir)/ref_guide/ref_guide.hh

# ----------------------- #
# User reference manual.  #
# ----------------------- #

all-local: $(srcdir)/$(USER_REFMAN) $(srcdir)/$(USER_REFMAN_PDF)

# Run "make regen-doc" to forcefully rebuild the Doxygen documentation.
.PHONY: regen-doc
regen-doc:
	-rm -f $(srcdir)/user-refman.stamp
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
	   -e 's,@top_builddir\@,$(top_builddir),g'		\
	   -e 's,@srcdir\@,$(srcdir),g'				\
	   -e 's,@builddir\@,$(builddir),g'

$(DOXYFILE_USER): $(srcdir)/$(DOXYFILE).in
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	  $(edit) $< >$@

## FIXME: Rename USER_REFMAN_PDF to something else?
USER_REFMAN_PDF = user-refman.pdf
## FIXME: Likewise?
USER_REFMAN = user-refman

ref-doc-pdf: $(srcdir)/$(USER_REFMAN_PDF)
ref-doc: $(srcdir)/$(USER_REFMAN)

# FIXME: Use texi2dvi instead of Doxygen's generated Makefile?
# (The LaTeX to PDF rule could be factored using a suffix rule, as in
# LRDE's share/).
$(srcdir)/$(USER_REFMAN_PDF): $(srcdir)/$(USER_REFMAN)
	cd $(srcdir)/$(USER_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(USER_REFMAN)/latex/refman.pdf $@

## FIXME: Are dependencies (`$(refman_dependencies)') set on the right
## rule?  Does Doxygen make a copy of figures, or does it generate
## LaTeX inputs relying on the existence of such figures in the
## initial location?  Investigate.
$(srcdir)/user-refman.stamp: $(srcdir)/$(DOXYFILE).in $(refman_dependencies)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(USER_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_USER)
	$(DOXYGEN) $(DOXYFILE_USER)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(USER_REFMAN).tmp/latex/refman.tex
	-rm -rf $(srcdir)/$(USER_REFMAN)
## FIXME: Moving directories between file systems is not portable.
	mv $(USER_REFMAN).tmp $(srcdir)/$(USER_REFMAN)
	@mv -f $@.tmp $@

$(srcdir)/$(USER_REFMAN): $(srcdir)/user-refman.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

# ---------------------------- #
# Developer reference manual.  #
# ---------------------------- #

# Run "make regen-devel-doc" to forcefully rebuild the Doxygen
# developer documentation.
.PHONY: regen-devel-doc
regen-devel-doc:
	-rm -f $(srcdir)/devel-refman.stamp
	$(MAKE) $(AM_MAKEFLAGS) $(srcdir)/devel-refman.stamp

DOXYFILE_DEVEL = Doxyfile_devel

edit_devel = sed							      \
  -e 's,OUTPUT_DIRECTORY       = @builddir@/user-refman.tmp/,OUTPUT_DIRECTORY       = @builddir@/devel-refman.tmp/,' \
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
	Id=`grep '^\$$Id' $(top_srcdir)/milena/ChangeLog	\
	      | sed -e 's/\\\$$//g'`;				\
	  $(edit) $< >$@.tmp
	$(edit_devel) $@.tmp >$@
	rm -f $@.tmp

## FIXME: Rename DEVEL_REFMAN_PDF to something else?
DEVEL_REFMAN_PDF = devel-refman-pdf
## FIXME: Likewise?
DEVEL_REFMAN = devel-refman

ref-doc-devel-pdf: $(srcdir)/$(DEVEL_REFMAN_PDF)
ref-doc-devel: $(srcdir)/$(DEVEL_REFMAN)

# Be warned, most LaTeX installations will not be able to compile the
# PDF development documentation as-is, as TeX's default memory
# capacities are usually too small to handle such a large document.
#
## FIXME: Use texi2dvi instead of Doxygen's generated Makefile?  (The
## LaTeX to PDF rule could be factored using a suffix rule, as in
## LRDE's share/).
$(srcdir)/$(DEVEL_REFMAN_PDF): $(srcdir)/$(DEVEL_REFMAN)
	cd $(srcdir)/$(DEVEL_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(DEVEL_REFMAN)/latex/refman.pdf $@

## FIXME: Are dependencies (`$(refman_dependencies)') set on the right
## rule?  Does Doxygen make a copy of figures, or does it generate
## LaTeX inputs relying on the existence of such figures in the
## initial location?  Investigate.
$(srcdir)/devel-refman.stamp: $(srcdir)/$(DOXYFILE).in $(refman_dependencies)
	@rm -f $@.tmp
	@touch $@.tmp
	-rm -rf $(DEVEL_REFMAN).tmp
	$(MAKE) $(AM_MAKE_FLAGS) $(DOXYFILE_DEVEL)
	$(DOXYGEN) $(DOXYFILE_DEVEL)
## Doxygen may generate an incomplete output and exit with success!
## Check some files before deeming the output as acceptable.
	test -f $(DEVEL_REFMAN).tmp/latex/refman.tex
	-rm -rf $(srcdir)/$(DEVEL_REFMAN)
## FIXME: Moving directories between file systems is not portable.
	mv $(DEVEL_REFMAN).tmp $(srcdir)/$(DEVEL_REFMAN)
	@mv -f $@.tmp $@

$(srcdir)/$(DEVEL_REFMAN): $(srcdir)/devel-refman.stamp
## Recover from the removal of $@
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi


# Clean Doxygen products.
maintainer-clean-local:
	-rm -rf $(USER_REFMAN).tmp $(srcdir)/$(USER_REFMAN)
	-rm -rf $(DEVEL_REFMAN).tmp $(srcdir)/$(DEVEL_REFMAN)
	-rm -f $(EXAMPLES_BUILDDIR)/*.cc
# FIXME: If we remove a *directory*, we are supposed to know how to
# recreate it later.  I doubt our rules are all currently able to
# handle this.  See how `make maintainer-clean all' behaves.
	-rm -rf $(OUTPUTS_BUILDDIR)
	-rm -rf $(FIGURES_BUILDDIR)


## ------------------------- ##
## Technical Documentation.  ##
## ------------------------- ##

.PHONY: technical technical-html technical-pdf

technical_TEXINPUTS ="$(DOC_SRCDIR):$(OUTPUTS_SRCDIR):$(srcdir):\
$(SPLIT_OUTPUTS_SRCDIR):$(IMG_SRCDIR):$(SPLIT_EXAMPLES_SRCDIR):"


technical: technical-html technical-pdf

technical_dir = $(doc_dir)/technical

# FIXME: Check these dependencies.  And it'd be better to depend on
# actual files rather than timestamps corresponding to a bunch of
# files.
technical_dependencies = $(doc_dir)/figures.stamp

# FIXME: As in milena/doc/Makefile.am, we should probably strip
# $(srcdir) prefixes from target variables, e.g. instead of:
#
#   FOO = $(srcdir)/foo.pdf
#   $(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# we should use:
#
#   FOO = foo.pdf
#   $(srcdir)/$(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# since it minimizes installation issues (see milena/doc/Makefile.am
# and Vaucanson's doc/Makefile.am).

# FIXME: Distributed products should be generated in the source dir.
# That's actually the case, *but* the current solution is not clean
# and might break sometimes.  The clean approach is to create a
# temporary directory, try to generate the documentation there, and
# move its contents to the source dir in case of success.  If the
# product is a directory, also refresh a timestamp (in the source
# dir).

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TECHNICAL_HH is meant to have a `.hh'
# extension, since it is later parsed by Doxygen, which complains
# about `.html' files.
TECHNICAL_HH = $(technical_dir)/technical.hh
technical-html: $(TECHNICAL_HH)
$(TECHNICAL_HH): $(technical_dir)/technical.tex $(technical_dependencies)
	$(doc_dir)/tools/todoxygen.sh $< $(technical_dir) $(doc_dir)


# Final product.
TECHNICAL_PDF = $(technical_dir)/technical.pdf
technical-pdf: $(TECHNICAL_PDF)
$(TECHNICAL_PDF): $(technical_dir)/technical.tex $(technical_dependencies)
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(technical_TEXINPUTS) pdflatex $<	\
	test "x$(technical_dir)" != x.			\
	  && mv -f technical.pdf $(technical_dir)

dist_doc_DATA += $(TECHNICAL_PDF)

EXTRA_DIST +=					\
  $(technical_dir)/technical.tex		\
  $(TECHNICAL_HH)

# FIXME: Remove `tutorial.aux tutorial.toc tutorial.log tutorial.bbl
# tutorial.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(TUTORIAL_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  technical.aux technical.toc technical.log technical.bbl technical.out	\
  *blg *.lot								\
  $(TECHNICAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  technical.html							\
  technical.idx								\
  $(TECHNICAL_HH)


## ---------- ##
## Tutorial.  ##
## ---------- ##

.PHONY: tutorial tutorial-html tutorial-pdf

tutorial_TEXINPUTS = $(DOC_SRCDIR):$(OUTPUTS_SRCDIR):$(IMG_SRCDIR):


tutorial: tutorial-html tutorial-pdf

tutorial_dir = $(doc_dir)/tutorial

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    corresponding to a bunch of files.
tutorial_dependencies =				\
  $(doc_dir)/split-examples.stamp		\
  $(doc_dir)/split-outputs.stamp		\
  $(doc_dir)/figures.stamp

# FIXME: As in milena/doc/Makefile.am, we should probably strip
# $(srcdir) prefixes from target variables, e.g. instead of:
#
#   FOO = $(srcdir)/foo.pdf
#   $(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# we should use:
#
#   FOO = foo.pdf
#   $(srcdir)/$(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# since it minimizes installation issues (see milena/doc/Makefile.am
# and Vaucanson's doc/Makefile.am).

# FIXME: Distributed products should be generated in the source dir.
# That's actually the case, *but* the current solution is not clean
# and might break sometimes.  The clean approach is to create a
# temporary directory, try to generate the documentation there, and
# move its contents to the source dir in case of success.  If the
# product is a directory, also refresh a timestamp (in the source
# dir).

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TUTORIAL_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
TUTORIAL_HH = $(tutorial_dir)/tutorial.hh
tutorial-html: $(TUTORIAL_HH)
$(TUTORIAL_HH): $(tutorial_dir)/tutorial.tex $(tutorial_dependencies)
	$(doc_dir)/tools/todoxygen.sh $< $(tutorial_dir) $(doc_dir)

# Final product.
TUTORIAL_PDF = $(tutorial_dir)/tutorial.pdf
tutorial-pdf: $(TUTORIAL_PDF)
$(TUTORIAL_PDF): $(tutorial_dir)/tutorial.tex $(tutorial_dependencies)
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(tutorial_TEXINPUTS) pdflatex $<	\
	test x"$(tutorial_dir)" != x.			\
	  && mv -f tutorial.pdf $(tutorial_dir)

dist_doc_DATA += $(TUTORIAL_PDF)

EXTRA_DIST +=					\
  $(tutorial_dir)/tutorial.tex			\
  $(TUTORIAL_HH)

# FIXME: Remove `tutorial.aux tutorial.toc tutorial.log tutorial.bbl
# tutorial.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(TUTORIAL_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out	\
  *blg *.lot								\
  $(TUTORIAL_PDF)							\
  *.haux *.hh *.html *.htoc						\
  tutorial.html								\
  tutorial.idx								\
  $(TUTORIAL_HH)


## ------------- ##
## White Paper.  ##
## ------------- ##

.PHONY: white-paper white-paper-html white-paper-pdf

white_paper_dir = $(doc_dir)/white_paper

white_paper_TEXINPUTS = $(DOC_SRCDIR):$(white_paper_dir):

PNGS =								\
  $(white_paper_dir)/figures/house.png				\
  $(white_paper_dir)/figures/house_rag.png			\
  $(white_paper_dir)/figures/house_wshed.png			\
  $(white_paper_dir)/figures/house_wshed_mean_colors.png

EPSS = $(PNGS:png=eps)

# FIXME: we want to check if 'figures' exists only once!
.png.eps:
	test -d figures || mkdir figures
	convert $< $@


white-paper: white-paper-pdf white-paper-html

# FIXME: As in milena/doc/Makefile.am, we should probably strip
# $(srcdir) prefixes from target variables, e.g. instead of:
#
#   FOO = $(srcdir)/foo.pdf
#   $(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# we should use:
#
#   FOO = foo.pdf
#   $(srcdir)/$(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# since it minimizes installation issues (see milena/doc/Makefile.am
# and Vaucanson's doc/Makefile.am).

# FIXME: Distributed products should be generated in the source dir.
# That's actually the case, *but* the current solution is not clean
# and might break sometimes.  The clean approach is to create a
# temporary directory, try to generate the documentation there, and
# move its contents to the source dir in case of success.  If the
# product is a directory, also refresh a timestamp (in the source
# dir).

# FIXME: Use texi2dvi/TeX4ht instead of plain hevea.
WHITE_PAPER_HTML = $(white_paper_dir)/white_paper_html/index.html
white-paper-html: $(WHITE_PAPER_HTML)
$(WHITE_PAPER_HTML): $(white_paper_dir)/white_paper.tex $(EPSS)
	test -d $(white_paper_dir)/white_paper_html	\
	  || mkdir $(white_paper_dir)/white_paper_html
	TEXINPUTS=$(white_paper_TEXINPUTS) hevea -O -fix $< -o $@
	$(doc_dir)/tools/clearbanner.sh $@


WHITE_PAPER_PDF = $(white_paper_dir)/white_paper.pdf
white-paper-pdf: $(WHITE_PAPER_PDF)
$(WHITE_PAPER_PDF): $(white_paper_dir)/white_paper.tex $(PNGS)
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(white_paper_TEXINPUTS) pdflatex $<	\
	test "x$(white_paper_dir)" != x.		\
	  && mv -f white_paper.pdf $(white_paper_dir)


dist_doc_DATA +=				\
  $(WHITE_PAPER_HTML)				\
  $(WHITE_PAPER_PDF)

EXTRA_DIST +=					\
  $(white_paper_dir)/white_paper.tex		\
  $(PNGS)					\
  $(EPSS)

# FIXME: Remove unnecessary patterns.
CLEANFILES +=					\
  white_paper_image.*				\
  white_paper.pdf				\
  *.log *.idx *.out *.aux

# FIXME: Likewise.
clean-local:
	-rm -rf white_paper_html figures


## ----------------- ##
## Reference Guide.  ##
## ----------------- ##

.PHONY: ref-guide ref-guide-html ref-guide-pdf

ref_guide_TEXINPUTS = "$(DOC_SRCDIR):$(OUTPUTS_SRCDIR):\
$(SPLIT_OUTPUTS_SRCDIR):$(IMG_SRCDIR):$(SPLIT_EXAMPLES_SRCDIR):"


ref-guide: ref-guide-html ref-guide-pdf

ref_guide_dir = $(doc_dir)/ref_guide

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    corresponding to a bunch of files.
ref_guide_dependencies =			\
  $(doc_dir)/split-examples.stamp		\
  $(doc_dir)/split-outputs.stamp		\
  $(doc_dir)/figures.stamp

# FIXME: As in milena/doc/Makefile.am, we should probably strip
# $(srcdir) prefixes from target variables, e.g. instead of:
#
#   FOO = $(srcdir)/foo.pdf
#   $(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# we should use:
#
#   FOO = foo.pdf
#   $(srcdir)/$(FOO): foo.tex bar.tex
#   dist_doc_DATA = $(FOO)
#
# since it minimizes installation issues (see milena/doc/Makefile.am
# and Vaucanson's doc/Makefile.am).

# FIXME: Distributed products should be generated in the source dir.
# That's actually the case, *but* the current solution is not clean
# and might break sometimes.  The clean approach is to create a
# temporary directory, try to generate the documentation there, and
# move its contents to the source dir in case of success.  If the
# product is a directory, also refresh a timestamp (in the source
# dir).

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: REF_GUIDE_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
REF_GUIDE_HH = $(ref_guide_dir)/ref_guide.hh
ref-guide-html: $(REF_GUIDE_HH)
$(REF_GUIDE_HH): $(ref_guide_dir)/ref_guide.tex $(ref_guide_dependencies)
	$(doc_dir)/tools/todoxygen.sh $< $(ref_guide_dir) $(doc_dir)


# Final product.
REF_GUIDE_PDF = $(ref_guide_dir)/ref_guide.pdf
ref-guide-pdf: $(REF_GUIDE_PDF)
$(REF_GUIDE_PDF): $(ref_guide_dir)/ref_guide.tex $(ref_guide_dependencies)
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<
	TEXINPUTS=$(ref_guide_TEXINPUTS) pdflatex $<	\
	test x"$(ref_guide_dir)" != x.			\
	  && mv -f ref_guide.pdf $(ref_guide_dir)

dist_doc_DATA += $(REF_GUIDE_PDF)

EXTRA_DIST +=					\
  $(ref_guide_dir)/ref_guide.tex		\
  $(REF_GUIDE_HH)

# FIXME: Remove `ref_guide.aux ref_guide.toc ref_guide.log
# ref_guide.bbl ref_guide.out *blg *.lot' when texi2dvi is used.
#
# FIXME: Why is $(REF_GUIDE_PDF) listed here?  It should be
# automatically cleaned.  I'm not sure either about the rest of
# CLEANFILES.
CLEANFILES +=								\
  ref_guide.aux ref_guide.toc ref_guide.log ref_guide.bbl ref_guide.out	\
  *blg *.lot								\
  $(REF_GUIDE_PDF)							\
  *.haux *.hh *.html *.htoc						\
  ref_guide.html							\
  ref_guide.idx								\
  $(REF_GUIDE_HH)


## --------- ##
## Figures.  ##
## --------- ##

$(srcdir)/figures/figures.mk: $(srcdir)/headers.stamp
include $(srcdir)/figures/figures.mk

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


## ---------- ##
## Examples.  ##
## ---------- ##

PROGRAMS_examples =		\
  accu-right-instanciation 	\
  borderthickness 		\
  box2d-bbox 			\
  domain-display 		\
  dpoint-1 			\
  estim-sites 			\
  extend 			\
  extension-ignore 		\
  fill 				\
  fill-call-1			\
  fill-imageif-cfun 		\
  fill-part-image 		\
  fill-subdomain 		\
  fill-subdomain-shorter	\
  first_routine 		\
  forall-piter  		\
  fun-p2v-1 			\
  graph-data 			\
  graph-iter 			\
  ima-has 			\
  ima-save 			\
  ima-size 			\
  ima2d-1 			\
  ima2d-2 			\
  ima2d-3 			\
  ima2d-4 			\
  ima2d-5 			\
  ima2d-6-clone			\
  ima2d-7 			\
  ima2d-rot 			\
  labeling-compute 		\
  logical-not 			\
  mln_var 			\
  parray-append 		\
  parray-bbox 			\
  paste 			\
  paste-call-1 			\
  point-1 			\
  predicate-1 			\
  win-create-1 			\
  win-create-2

PROGRAMS_examples += 	\
  tuto2_first_image 		\
  tuto3_rw_image 		\
  tuto4_genericity_and_algorithms

# Tuto3
PROGRAMS_examples += 	\
  tuto3_colorize 		\
  tuto3_println 		\
  tuto3_trace

# Tuto4
PROGRAMS_examples += 	\
  tuto4_point2d			\
  tuto4_site_set_create

EXTRA_PROGRAMS = $(PROGRAMS_examples)
CLEANFILES += $(PROGRAMS_examples)

# FIXME: Careful, this will only work if this file is included by
# `milena/doc/Makefile.am', since `examples/' is a subdirectory of
# `milena/doc/'.  Anyway, this file is to be eventually inlined in
# `milena/doc/Makefile.am', so this is not a big deal.
accu_right_instanciation_SOURCES = examples/accu-right-instanciation.cc
borderthickness_SOURCES = examples/borderthickness.cc
box2d_bbox_SOURCES = examples/box2d-bbox.cc
domain_display_SOURCES = examples/domain-display.cc
dpoint_1_SOURCES = examples/dpoint-1.cc
estim_sites_SOURCES = examples/estim-sites.cc
extend_SOURCES = examples/extend.cc
extension_ignore_SOURCES = examples/extension-ignore.cc
fill_SOURCES = examples/fill.cc
fill_call_1_SOURCES = examples/fill-call-1.cc
fill_imageif_cfun_SOURCES = examples/fill-imageif-cfun.cc
fill_part_image_SOURCES = examples/fill-part-image.cc
fill_subdomain_SOURCES = examples/fill-subdomain.cc
fill_subdomain_shorter_SOURCES = examples/fill-subdomain-shorter.cc
first_routine_SOURCES = examples/tuto3/first_routine.cc
forall_piter_SOURCES = examples/forall-piter.cc
fun_p2v_1_SOURCES = examples/fun-p2v-1.cc
graph_data_SOURCES = examples/graph-data.cc
graph_iter_SOURCES = examples/graph-iter.cc
ima2d_1_SOURCES = examples/ima2d-1.cc
ima2d_2_SOURCES = examples/ima2d-2.cc
ima2d_3_SOURCES = examples/ima2d-3.cc
ima2d_4_SOURCES = examples/ima2d-4.cc
ima2d_5_SOURCES = examples/ima2d-5.cc
ima2d_6_clone_SOURCES = examples/ima2d-6-clone.cc
ima2d_7_SOURCES = examples/ima2d-7.cc
ima2d_rot_SOURCES = examples/ima2d-rot.cc
ima_has_SOURCES = examples/ima-has.cc
ima_save_SOURCES = examples/ima-save.cc
ima_size_SOURCES = examples/ima-size.cc
labeling_compute_SOURCES = examples/labeling-compute.cc
logical_not_SOURCES = examples/logical-not.cc
mln_var_SOURCES = examples/mln_var.cc
parray_append_SOURCES = examples/parray-append.cc
parray_bbox_SOURCES = examples/parray-bbox.cc
paste_SOURCES = examples/paste.cc
paste_call_1_SOURCES = examples/paste-call-1.cc
point_1_SOURCES = examples/point-1.cc
predicate_1_SOURCES = examples/predicate-1.cc
win_create_1_SOURCES = examples/win-create-1.cc
win_create_2_SOURCES = examples/win-create-2.cc

tuto2_first_image_SOURCES = examples/tuto2_first_image.cc
tuto3_rw_image_SOURCES = examples/tuto3_rw_image.cc
tuto4_genericity_and_algorithms_SOURCES = examples/tuto4_genericity_and_algorithms.cc

# Tuto 3
tuto3_colorize_SOURCES = examples/tuto3/colorize.cc
tuto3_println_SOURCES = examples/tuto3/println.cc
tuto3_trace_SOURCES = examples/tuto3/trace.cc

# Tuto 4
tuto4_point2d_SOURCES = examples/tuto4/point2d.cc
tuto4_site_set_create_SOURCES = examples/tuto4/site_set_create.cc


# FIXME: Try to avoid the use an extensive list: either use Make's
# substitution rules or generate it.  Actually this list is already
# generated (see outputs/outputs.mk).
OUTPUTS =						\
  $(srcdir)/outputs/accu-right-instanciation.txt	\
  $(srcdir)/outputs/borderthickness.txt			\
  $(srcdir)/outputs/box2d-bbox.txt			\
  $(srcdir)/outputs/domain-display.txt			\
  $(srcdir)/outputs/dpoint-1.txt			\
  $(srcdir)/outputs/estim-sites.txt			\
  $(srcdir)/outputs/extend.txt				\
  $(srcdir)/outputs/extension-ignore.txt		\
  $(srcdir)/outputs/fill-call-1.txt			\
  $(srcdir)/outputs/fill-imageif-cfun.txt		\
  $(srcdir)/outputs/fill-part-image.txt			\
  $(srcdir)/outputs/fill-subdomain-shorter.txt		\
  $(srcdir)/outputs/fill-subdomain.txt			\
  $(srcdir)/outputs/fill.txt				\
  $(srcdir)/outputs/first_routine.txt			\
  $(srcdir)/outputs/forall-piter.txt			\
  $(srcdir)/outputs/fun-p2v-1.txt			\
  $(srcdir)/outputs/graph-data.txt			\
  $(srcdir)/outputs/graph-iter.txt			\
  $(srcdir)/outputs/graph-output-1.txt			\
  $(srcdir)/outputs/ima2d-1.txt				\
  $(srcdir)/outputs/ima2d-2.txt				\
  $(srcdir)/outputs/ima2d-3.txt				\
  $(srcdir)/outputs/ima2d-4.txt				\
  $(srcdir)/outputs/ima2d-5.txt				\
  $(srcdir)/outputs/ima2d-6-clone.txt			\
  $(srcdir)/outputs/ima2d-7.txt				\
  $(srcdir)/outputs/ima2d-decl-2.txt			\
  $(srcdir)/outputs/ima2d-display-1.txt			\
  $(srcdir)/outputs/ima2d-display-2.txt			\
  $(srcdir)/outputs/ima2d-display-output-1.txt		\
  $(srcdir)/outputs/ima2d-display-output-2.txt		\
  $(srcdir)/outputs/ima2d-rot.txt			\
  $(srcdir)/outputs/ima-has.txt				\
  $(srcdir)/outputs/ima-save.txt			\
  $(srcdir)/outputs/ima-size.txt			\
  $(srcdir)/outputs/labeling-compute.txt		\
  $(srcdir)/outputs/logical-not.txt			\
  $(srcdir)/outputs/mln_var.txt				\
  $(srcdir)/outputs/parray-append.txt			\
  $(srcdir)/outputs/parray-bbox.txt			\
  $(srcdir)/outputs/parray-display-1.txt		\
  $(srcdir)/outputs/paste-call-1.txt			\
  $(srcdir)/outputs/paste.txt				\
  $(srcdir)/outputs/point-1.txt				\
  $(srcdir)/outputs/predicate-1.txt			\
  $(srcdir)/outputs/tuto2_first_image.txt		\
  $(srcdir)/outputs/tuto3_colorize.txt			\
  $(srcdir)/outputs/tuto3_println.txt			\
  $(srcdir)/outputs/tuto3_rw_image.txt			\
  $(srcdir)/outputs/tuto3_trace.txt			\
  $(srcdir)/outputs/tuto4_genericity_and_algorithms.txt	\
  $(srcdir)/outputs/tuto4_point2d.txt			\
  $(srcdir)/outputs/tuto4_site_set_create.txt		\
  $(srcdir)/outputs/win-create-1-display.txt		\
  $(srcdir)/outputs/win-create-1.txt			\
  $(srcdir)/outputs/win-create-2.txt

$(DOC_SRCDIR)/examples-outputs.mk: $(DOC_SRCDIR)/gen-examples-outputs-mk $(srcdir)/Makefile.am
	$(DOC_SRCDIR)/gen-examples-outputs-mk $(PROGRAMS_examples) >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(DOC_SRCDIR)/examples-outputs.mk

MAINTAINERCLEANFILES += $(OUTPUTS)

# FIXME: Warning: $(SPLIT_EXAMPLES_SRCDIR) might not exist.  Ensure it
# exists.

# FIXME: Try to avoid the use an extensive list: either use Make's
# substitution rules or generate it.
EXAMPLES =						\
  $(srcdir)/examples/ima-save.cc			\
  $(srcdir)/examples/accu-right-instanciation.cc	\
  $(srcdir)/examples/borderthickness.cc			\
  $(srcdir)/examples/box2d-bbox.cc			\
  $(srcdir)/examples/domain-display.cc			\
  $(srcdir)/examples/dpoint-1.cc			\
  $(srcdir)/examples/estim-sites.cc			\
  $(srcdir)/examples/extend.cc				\
  $(srcdir)/examples/extension-ignore.cc		\
  $(srcdir)/examples/fill-call-1.cc			\
  $(srcdir)/examples/fill-imageif-cfun.cc		\
  $(srcdir)/examples/fill-part-image.cc			\
  $(srcdir)/examples/fill-subdomain-shorter.cc		\
  $(srcdir)/examples/fill-subdomain.cc			\
  $(srcdir)/examples/fill.cc				\
  $(srcdir)/examples/forall-piter.cc			\
  $(srcdir)/examples/fun-p2v-1.cc			\
  $(srcdir)/examples/graph-data.cc			\
  $(srcdir)/examples/graph-iter.cc			\
  $(srcdir)/examples/ima-has.cc				\
  $(srcdir)/examples/ima-load.cc			\
  $(srcdir)/examples/ima-size.cc			\
  $(srcdir)/examples/ima2d-1.cc				\
  $(srcdir)/examples/ima2d-2.cc				\
  $(srcdir)/examples/ima2d-3.cc				\
  $(srcdir)/examples/ima2d-4.cc				\
  $(srcdir)/examples/ima2d-5.cc				\
  $(srcdir)/examples/ima2d-6-clone.cc			\
  $(srcdir)/examples/ima2d-7.cc				\
  $(srcdir)/examples/ima2d-rot.cc			\
  $(srcdir)/examples/labeling-compute.cc		\
  $(srcdir)/examples/logical-not.cc			\
  $(srcdir)/examples/mln_var.cc				\
  $(srcdir)/examples/parray-append.cc			\
  $(srcdir)/examples/parray-bbox.cc			\
  $(srcdir)/examples/paste-call-1.cc			\
  $(srcdir)/examples/paste.cc				\
  $(srcdir)/examples/point-1.cc				\
  $(srcdir)/examples/predicate-1.cc			\
  $(srcdir)/examples/trash/accu.cc			\
  $(srcdir)/examples/trash/box.cc			\
  $(srcdir)/examples/trash/cpp_issue.cc			\
  $(srcdir)/examples/trash/erosion.cc			\
  $(srcdir)/examples/trash/for_Z.cc			\
  $(srcdir)/examples/trash/graph.cc			\
  $(srcdir)/examples/trash/image2d.cc			\
  $(srcdir)/examples/trash/image_flat.cc		\
  $(srcdir)/examples/trash/image_if.cc			\
  $(srcdir)/examples/trash/image_plain.cc		\
  $(srcdir)/examples/trash/image_safe.cc		\
  $(srcdir)/examples/trash/labeling_algo.cc		\
  $(srcdir)/examples/trash/mk_graph.cc			\
  $(srcdir)/examples/trash/p_array.2.cc			\
  $(srcdir)/examples/trash/p_array.cc			\
  $(srcdir)/examples/trash/p_image.cc			\
  $(srcdir)/examples/trash/p_key.2.cc			\
  $(srcdir)/examples/trash/p_key.cc			\
  $(srcdir)/examples/trash/p_line2d.cc			\
  $(srcdir)/examples/trash/p_mutable_array_of.cc	\
  $(srcdir)/examples/trash/p_queue.cc			\
  $(srcdir)/examples/trash/p_run.cc			\
  $(srcdir)/examples/trash/p_vaccess.cc			\
  $(srcdir)/examples/trash/proxy.cc			\
  $(srcdir)/examples/trash/sub_image.cc			\
  $(srcdir)/examples/trash/sub_image_if.cc		\
  $(srcdir)/examples/trash/tuto_bis.cc			\
  $(srcdir)/examples/trash/tuto_one.cc			\
  $(srcdir)/examples/trash/vec.cc			\
  $(srcdir)/examples/trash/win_multiple.cc		\
  $(srcdir)/examples/trash/window.cc			\
  $(srcdir)/examples/tuto2_first_image.cc		\
  $(srcdir)/examples/tuto3/colorize.cc			\
  $(srcdir)/examples/tuto3/first_routine.cc		\
  $(srcdir)/examples/tuto3/println.cc			\
  $(srcdir)/examples/tuto3/trace.cc			\
  $(srcdir)/examples/tuto3_rw_image.cc			\
  $(srcdir)/examples/tuto4/image.cc			\
  $(srcdir)/examples/tuto4/point2d.cc			\
  $(srcdir)/examples/tuto4/site_set_create.cc		\
  $(srcdir)/examples/tuto4_genericity_and_algorithms.cc	\
  $(srcdir)/examples/win-create-1.cc			\
  $(srcdir)/examples/win-create-2.cc

# FIXME: Place split files in their own directory, e.g.: split
# `examples/foo.cc' into `examples/split/foo.dir/foo-1.cc',
# `examples/split/foo.dir/foo-2.cc', etc.  It makes removal of these
# files easier.  We probably also nee a stamp file to keep track of
# dependencies.

# FIXME: Loops are bad, as they prevent parallelism.  And we prefer
# data-driven actions anyway.  This would imply that we know in
# advance the list out outputs (here, split examples).  Maybe we can
# infer it from the LaTeX document.
$(srcdir)/split-examples.stamp: $(EXAMPLES)
	@rm -f $@.tmp
	@touch $@.tmp
	failcom='exit 1';				\
	for file in $(EXAMPLES); do			\
	  $(DOC_SRCDIR)/tools/split_sample.sh		\
	      $$file cc raw $(SPLIT_EXAMPLES_SRCDIR)	\
	    || eval $$failcom;				\
	done
	@mv -f $@.tmp $@

# FIXME: Loops are bad, as they prevent parallelism.  And we prefer
# data-driven actions anyway.  This would imply that we know in
# advance the list out outputs (here, split outputs).  Maybe we can
# infer it from the LaTeX document.
$(srcdir)/split-outputs.stamp: $(OUTPUTS)
	@rm -f $@.tmp
	@touch $@.tmp
	failcom='exit 1';				\
	for file in $(OUTPUTS); do			\
	  $(DOC_SRCDIR)/tools/split_sample.sh		\
	      $$file txt "" $(SPLIT_OUTPUTS_SRCDIR)	\
	    || eval $$failcom;				\
	done
	@mv -f $@.tmp $@


# Regen Make helpers.
EXTRA_DIST += generate_dist_files.sh headers.stamp
include $(top_srcdir)/build-aux/regen-recursive.mk
## FIXME: It'd probably be better to separate the rules generating
## examples/examples.mk, outputs/outputs.mk and figures/figures.mk.
regen-am:
	@rm -f $(srcdir)/headers.stamp.tmp
	@touch $(srcdir)/headers.stamp.tmp
	@cd $(srcdir) && 				\
	  ./generate_dist_files.sh figures 'p*m' && 	\
	  ./generate_dist_files.sh outputs txt &&	\
	  ./generate_dist_files.sh examples cc.raw
	@mv -f $(srcdir)/headers.stamp.tmp $(srcdir)/headers.stamp
## FIXME: Regen examples-outputs.mk as well?

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


## -------------- ##
## Installation.  ##
## -------------- ##

# PDF.
dist_doc_DATA += $(USER_REFMAN_PDF)

# HTML.
# Installing a directory located in the source dir is not supported by
# Automake by default, so we cannot add `$(USER_REFMAN)/html' to
# `dist_doc_DATA'.  The following two rules are are workaround from
# the Vaucanson project.
EXTRA_DIST += $(USER_REFMAN)/html

# We install by hand, otherwise Automake produces "install .../srcdoc
# .../srcdoc", which installs our dir into the previous one, instead
# of replacing it.
install-data-local:
	rm -rf $(DESTDIR)$(htmldir)/$(USER_REFMAN)/html
	$(mkdir_p) $(DESTDIR)$(htmldir)
	cp -r $(srcdir)/$(USER_REFMAN)/html $(DESTDIR)$(htmldir)/user-refman
## Try GNU chmod's X mode before resorting to something slower but
## more standard.
	chmod -R a+rX $(DESTDIR)$(htmldir) || \
	find $(DESTDIR)$(htmldir) \( -type d -a -exec chmod a+rx {} \; \) \
             -o -exec chmod a+r {} \;

uninstall-local:
	chmod -R 700 $(DESTDIR)$(htmldir)/user-refman
	rm -rf $(DESTDIR)$(htmldir)/user-refman


EXTRA_DIST +=					\
  $(DOXYFILE).in				\
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

MAINTAINERCLEANFILES +=				\
  Doxyfile_user					\
  Doxyfile_devel
