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


## --------------------- ##
## LaTeX configuration.  ##
## --------------------- ##

include $(top_srcdir)/build-aux/tex.mk

# FIXME: Check this list.
TEXI2DVI_FLAGS +=							\
  -I $(DOC_SRCDIR) -I $(OUTPUTS_SRCDIR) -I $(SPLIT_OUTPUTS_SRCDIR)	\
  -I $(IMG_SRCDIR) -I $(SPLIT_EXAMPLES_SRCDIR)

# Illustrations of the white paper.
TEXI2DVI_FLAGS += -I $(DOC_SRCDIR)/white_paper

# Additional files for HeVeA compilations.
DOC_PACKAGES = $(srcdir)/doxyfuns.sty $(srcdir)/milena.sty


## --------- ##
## Figures.  ##
## --------- ##

# ----- #
# PBM.  #
# ----- #

$(srcdir)/pbm-figures.mk: $(srcdir)/gen-make-variable
	rm -f $@.tmp
	$(srcdir)/gen-make-variable PBM_FIGURES				  \
	  `find $(srcdir)/figures/*.pbm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/pbm-figures.mk
EXTRA_DIST += $(PBM_FIGURES)

.PHONY: regen-pbm-figures-mk
regen-pbm-figures-mk:
	rm -f $(srcdir)/pbm-figures.mk.tmp
	$(srcdir)/gen-make-variable PBM_FIGURES				  \
	  `find $(srcdir)/figures/*.pbm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$(srcdir)/pbm-figures.mk.tmp
	mv -f $(srcdir)/pbm-figures.mk.tmp $(srcdir)/pbm-figures.mk
	chmod -w $(srcdir)/pbm-figures.mk

# ----- #
# PGM.  #
# ----- #

$(srcdir)/pgm-figures.mk: $(srcdir)/gen-make-variable
	rm -f $@.tmp
	$(srcdir)/gen-make-variable PGM_FIGURES				  \
	  `find $(srcdir)/figures/*.pgm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/pgm-figures.mk
EXTRA_DIST += $(PGM_FIGURES)

.PHONY: regen-pgm-figures-mk
regen-pgm-figures-mk:
	rm -f $(srcdir)/pgm-figures.mk.tmp
	$(srcdir)/gen-make-variable PGM_FIGURES				  \
	  `find $(srcdir)/figures/*.pgm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$(srcdir)/pgm-figures.mk.tmp
	mv -f $(srcdir)/pgm-figures.mk.tmp $(srcdir)/pgm-figures.mk
	chmod -w $(srcdir)/pgm-figures.mk

# ----- #
# PPM.  #
# ----- #

$(srcdir)/ppm-figures.mk: $(srcdir)/gen-make-variable
	rm -f $@.tmp
	$(srcdir)/gen-make-variable PPM_FIGURES				  \
	  `find $(srcdir)/figures/*.ppm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/ppm-figures.mk
EXTRA_DIST += $(PPM_FIGURES)

.PHONY: regen-ppm-figures-mk
regen-ppm-figures-mk:
	rm -f $(srcdir)/ppm-figures.mk.tmp
	$(srcdir)/gen-make-variable PPM_FIGURES				  \
	  `find $(srcdir)/figures/*.ppm | sed 's,^$(srcdir),\$$(srcdir),'` \
	  >$(srcdir)/ppm-figures.mk.tmp
	mv -f $(srcdir)/ppm-figures.mk.tmp $(srcdir)/ppm-figures.mk
	chmod -w $(srcdir)/ppm-figures.mk

# ----- #
# PNG.  #
# ----- #

convert_to_png = convert -scale 250 -bordercolor black -border 2x2

SUFFIXES += .pbm .pgm .ppm .png

.pbm.png:
	$(convert_to_png) $< $@

.pgm.png:
	$(convert_to_png) $< $@

.ppm.png:
	$(convert_to_png) $< $@

PNG_FIGURES =					\
  $(PBM_FIGURES:.pbm=.png)			\
  $(PGM_FIGURES:.pgm=.png)			\
  $(PPM_FIGURES:.ppm=.png)
EXTRA_DIST += $(PNG_FIGURES)
MAINTAINERCLEANFILES = $(PNG_FIGURES)


## ---------- ##
## Examples.  ##
## ---------- ##

$(srcdir)/examples.mk: $(srcdir)/gen-make-variable
	rm -f $@.tmp
	$(srcdir)/gen-make-variable EXAMPLES	\
	  `find $(srcdir)/examples -name '*.cc'	\
	     | sed 's,^$(srcdir),\$$(srcdir),'`	\
	  >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/examples.mk
EXTRA_DIST += $(EXAMPLES)

.PHONY: regen-examples-mk
regen-examples-mk:
	rm -f $(srcdir)/examples.mk.tmp
	$(srcdir)/gen-make-variable EXAMPLES	\
	  `find $(srcdir)/examples -name '*.cc'	\
	     | sed 's,^$(srcdir),\$$(srcdir),'`	\
	  >$(srcdir)/examples.mk.tmp
	mv -f $(srcdir)/examples.mk.tmp $(srcdir)/examples.mk
	chmod -w $(srcdir)/examples.mk

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
  $(PNG_FIGURES)				\
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
MAINTAINERCLEANFILES += $(DOXYFILE_USER)

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

EXTRA_DIST += $(srcdir)/user-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/user-refman.stamp
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
MAINTAINERCLEANFILES += $(DOXYFILE_DEVEL)

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

EXTRA_DIST += $(srcdir)/devel-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/devel-refman.stamp
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

technical: technical-html technical-pdf

technical_dir = $(doc_dir)/technical

# FIXME: Check these dependencies.  And it'd be better to depend on
# actual files rather than timestamps corresponding to a bunch of
# files.
technical_dependencies = $(PNG_FIGURES)

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

## FIXME: technical technical.hh does not seem to be used (by the
## Doyxgen documentation); remove its generation? Careful,
## technical/technical.html *is* probably required, though.

# Final product.
TECHNICAL_HTML = $(technical_dir)/technical.html
TECHNICAL_HTML_OUTPUTS = $(TECHNICAL_HTML) $(technical_dir)/technical.css

EXTRA_DIST += $(TECHNICAL_HTML).stamp
MAINTAINERCLEANFILES += $(TECHNICAL_HTML).stamp
# HTML outputs contains several files (the HTML file itself, its CSS
# style sheet, possibly some images).  Therefore, we cannot rely on a
# simple suffix rule to produce them.
$(TECHNICAL_HTML).stamp: $(technical_dir)/technical.tex $(technical_dependencies)
	@rm -f $@.tmp
	@touch $@.tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) -o $(TECHNICAL_HTML) $<
	@mv -f $@.tmp $@

$(TECHNICAL_HTML_OUTPUTS): $(TECHNICAL_HTML).stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

dist_doc_DATA += $(TECHNICAL_HTML_OUTPUTS)
MAINTAINERCLEANFILES += $(TECHNICAL_HTML_OUTPUTS)

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TECHNICAL_HH is meant to have a `.hh'
# extension, since it is later parsed by Doxygen, which complains
# about `.html' files.
TECHNICAL_HH = $(technical_dir)/technical.hh
# FIXME: Careful, technical-html depends on $(TECHNICAL_HH), not
# $(TECHNICAL_HTML).  Anyway, this shortcut should vanish soon.
technical-html: $(TECHNICAL_HH)
$(TECHNICAL_HH): $(TECHNICAL_HTML) $(doc_dir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(doc_dir)/tools/todoxygen.sh $< $@.tmp $(doc_dir)
	mv -f $@.tmp $@


# Final product.
TECHNICAL_PDF = $(technical_dir)/technical.pdf
technical-pdf: $(TECHNICAL_PDF)
# Extra dependencies.
$(TECHNICAL_PDF): $(technical_dependencies)

dist_doc_DATA += $(TECHNICAL_PDF)
MAINTAINERCLEANFILES += $(TECHNICAL_PDF)

EXTRA_DIST +=					\
  $(technical_dir)/technical.tex		\
  $(TECHNICAL_HH)

MAINTAINERCLEANFILES += $(TECHNICAL_HH)

## ---------- ##
## Tutorial.  ##
## ---------- ##

.PHONY: tutorial tutorial-html tutorial-pdf

tutorial: tutorial-html tutorial-pdf

tutorial_dir = $(doc_dir)/tutorial

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    corresponding to a bunch of files.
tutorial_dependencies =				\
  $(doc_dir)/split-examples.stamp		\
  $(doc_dir)/split-outputs.stamp		\
  $(PNG_FIGURES)

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

# Final product.
TUTORIAL_HTML = $(tutorial_dir)/tutorial.html
TUTORIAL_HTML_OUTPUTS = $(TUTORIAL_HTML) $(tutorial_dir)/tutorial.css

EXTRA_DIST += $(TUTORIAL_HTML).stamp
MAINTAINERCLEANFILES += $(TUTORIAL_HTML).stamp
# HTML outputs contains several files (the HTML file itself, its CSS
# style sheet, possibly some images).  Therefore, we cannot rely on a
# simple suffix rule to produce them.
$(TUTORIAL_HTML).stamp: $(tutorial_dir)/tutorial.tex $(tutorial_dependencies)
	@rm -f $@.tmp
	@touch $@.tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) -o $(TUTORIAL_HTML) $<
	@mv -f $@.tmp $@

$(TUTORIAL_HTML_OUTPUTS): $(TUTORIAL_HTML).stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

dist_doc_DATA += $(TUTORIAL_HTML_OUTPUTS)
MAINTAINERCLEANFILES += $(TUTORIAL_HTML_OUTPUTS)

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TUTORIAL_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
TUTORIAL_HH = $(tutorial_dir)/tutorial.hh
# FIXME: Careful, tutorial-html depends on $(TUTORIAL_HH), not
# $(TUTORIAL_HTML).  Anyway, this shortcut should vanish soon.
tutorial-html: $(TUTORIAL_HH)
$(TUTORIAL_HH): $(TUTORIAL_HTML) $(doc_dir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(doc_dir)/tools/todoxygen.sh $< $@.tmp $(doc_dir)
	mv -f $@.tmp $@


# Final product.
TUTORIAL_PDF = $(tutorial_dir)/tutorial.pdf
tutorial-pdf: $(TUTORIAL_PDF)
# Extra dependencies.
$(TUTORIAL_PDF): $(tutorial_dependencies)

dist_doc_DATA += $(TUTORIAL_PDF)
MAINTAINERCLEANFILES += $(TUTORIAL_PDF)

EXTRA_DIST +=					\
  $(tutorial_dir)/tutorial.tex			\
  $(TUTORIAL_HH)

MAINTAINERCLEANFILES += $(TUTORIAL_HH)


## ------------- ##
## White Paper.  ##
## ------------- ##

.PHONY: white-paper white-paper-html white-paper-pdf

white_paper_dir = $(doc_dir)/white_paper

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

# Final product.
WHITE_PAPER_HTML = $(white_paper_dir)/white_paper.html
white-paper-html: $(WHITE_PAPER_HTML)
WHITE_PAPER_HTML_OUTPUTS =			\
  $(WHITE_PAPER_HTML)				\
  $(white_paper_dir)/white_paper.css		\
  $(white_paper_dir)/white_paper0x.png		\
  $(white_paper_dir)/white_paper1x.png		\
  $(white_paper_dir)/white_paper2x.png		\
  $(white_paper_dir)/white_paper3x.png

EXTRA_DIST += $(WHITE_PAPER_HTML).stamp
MAINTAINERCLEANFILES += $(WHITE_PAPER_HTML).stamp
# HTML outputs contains several files (the HTML file itself, its CSS
# style sheet, possibly some images).  Therefore, we cannot rely on a
# simple suffix rule to produce them.
$(WHITE_PAPER_HTML).stamp: $(white_paper_dir)/white_paper.tex $(EPSS)
	@rm -f $@.tmp
	@touch $@.tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) -o $(WHITE_PAPER_HTML) $<
	@mv -f $@.tmp $@

$(WHITE_PAPER_HTML_OUTPUTS): $(WHITE_PAPER_HTML).stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

dist_doc_DATA += $(WHITE_PAPER_HTML_OUTPUTS)
MAINTAINERCLEANFILES += $(WHITE_PAPER_HTML_OUTPUTS)

WHITE_PAPER_PDF = $(white_paper_dir)/white_paper.pdf
white-paper-pdf: $(WHITE_PAPER_PDF)
# Extra dependencies.
$(WHITE_PAPER_PDF): $(PNGS)

dist_doc_DATA += $(WHITE_PAPER_PDF)
MAINTAINERCLEANFILES += $(WHITE_PAPER_PDF)

EXTRA_DIST +=					\
  $(white_paper_dir)/white_paper.tex		\
  $(PNGS)					\
  $(EPSS)

MAINTAINERCLEANFILES += $(EPSS)


## ----------------- ##
## Reference Guide.  ##
## ----------------- ##

.PHONY: ref-guide ref-guide-html ref-guide-pdf

ref-guide: ref-guide-html ref-guide-pdf

ref_guide_dir = $(doc_dir)/ref_guide

# FIXME: Check these dependencies.
# 1. They might be redundant.
# 2. It'd be better to depend on actual files rather than timestamps
#    corresponding to a bunch of files.
ref_guide_dependencies =			\
  $(doc_dir)/split-examples.stamp		\
  $(doc_dir)/split-outputs.stamp		\
  $(PNG_FIGURES)

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

# Final product.
REF_GUIDE_HTML = $(ref_guide_dir)/ref_guide.html
REF_GUIDE_HTML_OUTPUTS =			\
  $(REF_GUIDE_HTML)				\
  $(ref_guide_dir)/ref_guide.css		\
  $(ref_guide_dir)/ref_guide0x.png

EXTRA_DIST += $(REF_GUIDE_HTML).stamp
MAINTAINERCLEANFILES += $(REF_GUIDE_HTML).stamp
# HTML outputs contains several files (the HTML file itself, its CSS
# style sheet, possibly some images).  Therefore, we cannot rely on a
# simple suffix rule to produce them.
$(REF_GUIDE_HTML).stamp: $(ref_guide_dir)/ref_guide.tex $(ref_guide_dependencies)
	@rm -f $@.tmp
	@touch $@.tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) -o $(REF_GUIDE_HTML) $<
	@mv -f $@.tmp $@

$(REF_GUIDE_HTML_OUTPUTS): $(REF_GUIDE_HTML).stamp
## Recover from the removal of $@
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi

dist_doc_DATA += $(REF_GUIDE_HTML_OUTPUTS)
MAINTAINERCLEANFILES += $(REF_GUIDE_HTML_OUTPUTS)

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: REF_GUIDE_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
REF_GUIDE_HH = $(ref_guide_dir)/ref_guide.hh
# FIXME: Careful, ref-guide-html depends on $(REF_GUIDE_HH), not
# $(REF_GUIDE_HTML).  Anyway, this shortcut should vanish soon.
ref-guide-html: $(REF_GUIDE_HH)
$(REF_GUIDE_HH): $(REF_GUIDE_HTML) $(doc_dir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(doc_dir)/tools/todoxygen.sh $< $@.tmp $(doc_dir)
	mv -f $@.tmp $@


# Final product.
REF_GUIDE_PDF = $(ref_guide_dir)/ref_guide.pdf
ref-guide-pdf: $(REF_GUIDE_PDF)
# Extra dependencies.
$(REF_GUIDE_PDF): $(ref_guide_dependencies)

dist_doc_DATA += $(REF_GUIDE_PDF)
MAINTAINERCLEANFILES += $(REF_GUIDE_PDF)

EXTRA_DIST +=					\
  $(ref_guide_dir)/ref_guide.tex		\
  $(REF_GUIDE_HH)

MAINTAINERCLEANFILES += $(REF_GUIDE_HH)


## ---------- ##
## Programs.  ##
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
CLEANFILES = $(PROGRAMS_examples)

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
  $(srcdir)/outputs/ima-has.txt				\
  $(srcdir)/outputs/ima-save.txt			\
  $(srcdir)/outputs/ima-size.txt			\
  $(srcdir)/outputs/ima2d-1.txt				\
  $(srcdir)/outputs/ima2d-2.txt				\
  $(srcdir)/outputs/ima2d-3.txt				\
  $(srcdir)/outputs/ima2d-4.txt				\
  $(srcdir)/outputs/ima2d-5.txt				\
  $(srcdir)/outputs/ima2d-6-clone.txt			\
  $(srcdir)/outputs/ima2d-7.txt				\
  $(srcdir)/outputs/ima2d-rot.txt			\
  $(srcdir)/outputs/labeling-compute.txt		\
  $(srcdir)/outputs/logical-not.txt			\
  $(srcdir)/outputs/mln_var.txt				\
  $(srcdir)/outputs/parray-append.txt			\
  $(srcdir)/outputs/parray-bbox.txt			\
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
  $(srcdir)/outputs/win-create-1.txt			\
  $(srcdir)/outputs/win-create-2.txt

## FIXME: These outpouts are also required by the (LaTeX)
## documentation.  However, they have no corresponding program in
## examples/, hence they cannot be regenerated.  Are they hand-made
## outputs?  Or maybe outputs of which program has been removed since?
MORE_OUTPUTS =					\
  $(srcdir)/outputs/graph-output-1.txt		\
  $(srcdir)/outputs/ima2d-display-1.txt		\
  $(srcdir)/outputs/ima2d-display-2.txt		\
  $(srcdir)/outputs/ima2d-display-output-1.txt	\
  $(srcdir)/outputs/ima2d-display-output-2.txt	\
  $(srcdir)/outputs/parray-display-1.txt	\
  $(srcdir)/outputs/win-create-1-display.txt

$(DOC_SRCDIR)/examples-outputs.mk: $(DOC_SRCDIR)/gen-examples-outputs-mk $(srcdir)/Makefile.am
	$(DOC_SRCDIR)/gen-examples-outputs-mk $(PROGRAMS_examples) >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(DOC_SRCDIR)/examples-outputs.mk

.PHONY: regen-examples-outputs-mk
regen-examples-outputs-mk:
	$(DOC_SRCDIR)/gen-examples-outputs-mk $(PROGRAMS_examples)	\
	   >$(DOC_SRCDIR)/examples-outputs.mk.tmp
	mv -f $(DOC_SRCDIR)/examples-outputs.mk.tmp	\
	  $(DOC_SRCDIR)/examples-outputs.mk
	chmod -w $(DOC_SRCDIR)/examples-outputs.mk

MAINTAINERCLEANFILES += $(OUTPUTS)

# FIXME: Warning: $(SPLIT_EXAMPLES_SRCDIR) might not exist.  Ensure it
# exists.

# FIXME: Place split files in their own directory, e.g.: split
# `examples/foo.cc' into `examples/split/foo.dir/foo-1.cc',
# `examples/split/foo.dir/foo-2.cc', etc.  It makes removal of these
# files easier.  We probably also nee a stamp file to keep track of
# dependencies.

EXTRA_DIST += $(srcdir)/split-examples.stamp
MAINTAINERCLEANFILES += $(srcdir)/split-examples.stamp
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

EXTRA_DIST += $(srcdir)/split-outputs.stamp
MAINTAINERCLEANFILES += $(srcdir)/split-outputs.stamp
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


## -------------- ##
## Regeneration.  ##
## -------------- ##

# Regen Make helpers.
EXTRA_DIST += generate_dist_files.sh headers.stamp
include $(top_srcdir)/build-aux/regen-recursive.mk
## FIXME: Get rid of headers.stamp, which is (was) used by two many
## generators.
regen-am:
	@rm -f $(srcdir)/headers.stamp.tmp
	@touch $(srcdir)/headers.stamp.tmp
	$(MAKE) $(AM_MAKEFLAGS) regen-pbm-figures-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-pgm-figures-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-ppm-figures-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-outputs-mk
	@cd $(srcdir) && 				\
	  ./generate_dist_files.sh outputs txt &&	\
	  ./generate_dist_files.sh examples cc.raw
	@mv -f $(srcdir)/headers.stamp.tmp $(srcdir)/headers.stamp

$(srcdir)/examples/examples.mk: $(srcdir)/headers.stamp
$(srcdir)/outputs/outputs.mk: $(srcdir)/headers.stamp


$(srcdir)/headers.stamp: $(srcdir)/generate_dist_files.sh
	@rm -f $@.tmp
	@touch $@.tmp
	@cd $(srcdir) && 				\
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

# Generate a Makefile helper containing a variable assignment.
EXTRA_DIST += gen-make-variable
# FIXME: Can't we implement this using `gen-make-variable'?
EXTRA_DIST += gen-examples-outputs-mk
