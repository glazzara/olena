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
# Find tests/data.hh and doc/tools/data.hh (generated headers).
AM_CPPFLAGS += -I$(top_builddir)/milena

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


## --------- ##
## Figures.  ##
## --------- ##

# Generate a Makefile helper containing the list of generated figures.
EXTRA_DIST = gen-figures-mk
$(srcdir)/figures.mk: $(srcdir)/gen-figures-mk
	rm -f $@.tmp
	cd $(srcdir) && ./gen-figures-mk >figures.mk.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/figures.mk
EXTRA_DIST += $(FIGURES)
MAINTAINERCLEANFILES = $(FIGURES)

.PHONY: regen-figures-mk
regen-figures-mk:
	rm -f $(srcdir)/figures.mk.tmp
	cd $(srcdir) && ./gen-figures-mk >figures.mk.tmp
	mv -f $(srcdir)/figures.mk.tmp $(srcdir)/figures.mk
	chmod -w $(srcdir)/figures.mk

# ------------- #
# PNG figures.  #
# ------------- #

# PNG figures are generated from PBM, PGM and PPM figures.

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
  $(PPM_FIGURES:.ppm=.png)			\
  $(srcdir)/img/genericity.png 			\
  $(srcdir)/img/2dgrid.png
EXTRA_DIST += $(PNG_FIGURES)
MAINTAINERCLEANFILES += $(PNG_FIGURES)

# Prevent make from trying to regenerate img/small.png from
# img/small.pbm through suffix rule `.pbm.png', as they are not
# related, by providing the following dummy rule.  A better way would
# be to rename one these files to break this spurious dependency.
$(srcdir)/img/small.png:
	@:

# --------------- #
# Other figures.  #
# --------------- #

TECHNICAL_FIGURES =				\
  figures/technical/file_layout.pdf		\
  figures/technical/file_layout.png
MORE_FIGURES = $(TECHNICAL_FIGURES)

WHITE_PAPER_FIGURES =				\
  $(srcdir)/figures/house.png			\
  $(srcdir)/figures/house_rag.png		\
  $(srcdir)/figures/house_wshed.png		\
  $(srcdir)/figures/house_wshed_mean_colors.png
MORE_FIGURES += $(WHITE_PAPER_FIGURES)

# This image seems to be used nowhere.
MORE_FIGURES += figures/accu_image_morpher_big_picture.pdf

EXTRA_DIST += $(MORE_FIGURES)


## ---------- ##
## Examples.  ##
## ---------- ##

# Generate a Makefile helper containing a variable assignment.
EXTRA_DIST += gen-make-variable

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

# Examples used in the technical documentation.
TECHNICAL_EXAMPLES =				\
  examples/devel/dispatch.cc.raw		\
  examples/devel/facade.cc.raw			\
  examples/devel/impl.cc.raw
EXTRA_DIST += $(TECHNICAL_EXAMPLES)

# Non generated examples.
MORE_EXAMPLES =					\
  examples/accu-wrong-instantiation.cc.raw	\
  examples/fill-proto.cc.raw			\
  examples/ima2d-restricted-1.cc.raw		\
  examples/ima2d-restricted-2.cc.raw		\
  examples/ima2d-restricted-3.cc.raw		\
  examples/tuto3/fill_non_generic.cc.raw
EXTRA_DIST += $(MORE_EXAMPLES)

## ---------------- ##
## Split examples.  ##
## ---------------- ##

# Generate a Makefile helper handling the generation of split examples.
EXTRA_DIST += gen-split-examples-mk
$(srcdir)/split-examples.mk: $(srcdir)/gen-split-examples-mk
	rm -f $@.tmp
	cd $(srcdir) && ./gen-split-examples-mk >split-examples.mk.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/split-examples.mk
EXTRA_DIST += $(SPLIT_EXAMPLES)
MAINTAINERCLEANFILES += $(SPLIT_EXAMPLES)

.PHONY: regen-split-examples-mk
regen-split-examples-mk:
	rm -f $(srcdir)/split-examples.mk.tmp
	cd $(srcdir) && ./gen-split-examples-mk >split-examples.mk.tmp
	mv -f $(srcdir)/split-examples.mk.tmp $(srcdir)/split-examples.mk
	chmod -w $(srcdir)/split-examples.mk


## ----------------------- ##
## Doxygen documentation.  ##
## ----------------------- ##

REFMAN_deps =					\
  $(PNG_FIGURES)				\
  $(srcdir)/tutorial.dox			\
  $(srcdir)/ref-guide.dox

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
	   -e 's,@builddir[@],$(builddir),g'			\
	   -e 's,@boostcppflags[@],$(BOOST_CPPFLAGS),g'

$(DOXYFILE_USER): $(srcdir)/$(DOXYFILE).in
	$(edit) $< >$@
MAINTAINERCLEANFILES += $(DOXYFILE_USER)

USER_REFMAN = user-refman


# Disable PDF output since the file is too large to be processed
# correctly by latex with the default configuration. This file is also
# too large to be effectively usable by the user.

# PDF.
#USER_REFMAN_PDF = user-refman.pdf
#dist_pdf_DATA = $(srcdir)/$(USER_REFMAN_PDF)
dist_pdf_DATA =

#$(srcdir)/$(USER_REFMAN_PDF): $(srcdir)/$(USER_REFMAN)
#	cd $(srcdir)/$(USER_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
#	cp -f $(srcdir)/$(USER_REFMAN)/latex/refman.pdf $@

# HTML.
EXTRA_DIST += $(srcdir)/user-refman.stamp
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
	  perl -pi -e "s,@relcommondocpath[@],../../../../doc/,g;"	\
	           -e "s,@userrefman[@],$(USER_REFMAN),g"		\
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
  -e 's,OUTPUT_DIRECTORY       = \(.*\)/user-refman.tmp,OUTPUT_DIRECTORY       = \1/devel-refman.tmp,' \
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
  -e 's,PROJECT_NUMBER         = \"User documentation ,PROJECT_NUMBER         = \"Developer documentation ,' \
  -e 's,EXCLUDE_SYMBOLS	       = \*::internal\* \*_ mln::trait::\*,EXCLUDE_SYMBOLS	       =,'

$(DOXYFILE_DEVEL): $(srcdir)/$(DOXYFILE).in
	rm -f $@.tmp
	$(edit) $< >$@.tmp
	$(edit_devel) $@.tmp >$@
	rm -f $@.tmp
MAINTAINERCLEANFILES += $(DOXYFILE_DEVEL)

DEVEL_REFMAN = devel-refman

# Disable PDF output since the file is too large to be processed
# correctly by latex with the default configuration. This file is also
# too large to be effectively usable by the user.

# PDF.
#DEVEL_REFMAN_PDF = devel-refman.pdf
# Be warned, most LaTeX installations will not be able to compile the
# PDF development documentation as-is, as TeX's default memory
# capacities are usually too small to handle such a large document.
#$(srcdir)/$(DEVEL_REFMAN_PDF): $(srcdir)/$(DEVEL_REFMAN)
#	cd $(srcdir)/$(DEVEL_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
#	cp -f $(srcdir)/$(DEVEL_REFMAN)/latex/refman.pdf $@

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
	perl -pi \
	     -e "s,@relcommondocpath[@],$(top_srcdir)/doc/$(DEVEL_REFMAN)/html/,g" \
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


## ------------------------- ##
## Technical documentation.  ##
## ------------------------- ##

EXTRA_DIST += technical.tex

TECHNICAL_deps =				\
  $(TECHNICAL_FIGURES)				\
  $(TECHNICAL_EXAMPLES)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

# PDF.
TECHNICAL_PDF = $(srcdir)/technical.pdf
dist_pdf_DATA += $(TECHNICAL_PDF)
$(TECHNICAL_PDF): $(TECHNICAL_deps)

# HTML.
TECHNICAL = technical
all-local: $(srcdir)/$(TECHNICAL).stamp
# The HTML output contains several files (the HTML file itself, its
# CSS style sheet, possibly some images).  Therefore, we cannot rely
# on a simple suffix rule to produce them.  Instead, put the products
# in a directory and use a time stamp.
$(srcdir)/$(TECHNICAL).stamp: $(srcdir)/technical.tex $(TECHNICAL_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(TECHNICAL).tmp
	mkdir $(TECHNICAL).tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS) \
	  -o $(TECHNICAL).tmp/$(TECHNICAL).html $<
	rm -rf $(srcdir)/$(TECHNICAL)
## FIXME: moving directories between file systems is not portable.
	mv $(TECHNICAL).tmp $(srcdir)/$(TECHNICAL)
	@mv -f $@.tmp $@
EXTRA_DIST += $(srcdir)/$(TECHNICAL).stamp
MAINTAINERCLEANFILES += $(srcdir)/$(TECHNICAL).stamp

$(srcdir)/$(TECHNICAL): $(srcdir)/$(TECHNICAL).stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
DATA_html_dirs = $(TECHNICAL)


## ---------- ##
## Tutorial.  ##
## ---------- ##

EXTRA_DIST += tutorial.tex

TUTORIAL_deps =					\
  $(EXAMPLES)					\
  $(SPLIT_EXAMPLES)				\
  $(OUTPUTS)					\
  $(SPLIT_OUTPUTS)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

# PDF.
TUTORIAL_PDF = $(srcdir)/tutorial.pdf
dist_pdf_DATA += $(TUTORIAL_PDF)
$(TUTORIAL_PDF): $(TUTORIAL_deps)

# HTML.
TUTORIAL = tutorial
all-local: $(srcdir)/$(TUTORIAL).stamp

# The HTML output contains several files (the HTML file itself, its
# CSS style sheet, possibly some images).  Therefore, we cannot rely
# on a simple suffix rule to produce them.  Instead, put the products
# in a directory and use a time stamp.
$(srcdir)/$(TUTORIAL).stamp: $(srcdir)/tutorial.tex $(TUTORIAL_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(TUTORIAL).tmp
	mkdir $(TUTORIAL).tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS)			\
	  -o $(TUTORIAL).tmp/$(TUTORIAL).html $<
	rm -rf $(srcdir)/$(TUTORIAL)
## FIXME: moving directories between file systems is not portable.
	mv $(TUTORIAL).tmp $(srcdir)/$(TUTORIAL)
	@mv -f $@.tmp $@
EXTRA_DIST += $(srcdir)/$(TUTORIAL).stamp
MAINTAINERCLEANFILES += $(srcdir)/$(TUTORIAL).stamp

$(srcdir)/$(TUTORIAL): $(srcdir)/$(TUTORIAL).stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
DATA_html_dirs += $(TUTORIAL)

# Intermediate product for the (Doxygen) User Reference Manual.
TUTORIAL_HH = $(srcdir)/tutorial.dox
$(TUTORIAL_HH): $(srcdir)/$(TUTORIAL).stamp $(srcdir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(srcdir)/tools/todoxygen.sh					\
	  $(srcdir)/$(TUTORIAL)/$(TUTORIAL).html $@.tmp $(srcdir)
	mv -f $@.tmp $@
EXTRA_DIST += $(TUTORIAL_HH)
MAINTAINERCLEANFILES += $(TUTORIAL_HH)


## ------------- ##
## White Paper.  ##
## ------------- ##

EXTRA_DIST += white-paper.tex graphicx.hva

WHITE_PAPER_deps =				\
  $(WHITE_PAPER_FIGURES)			\
  graphicx.hva

# PDF.
WHITE_PAPER_PDF = $(srcdir)/white-paper.pdf
dist_pdf_DATA += $(WHITE_PAPER_PDF)
$(WHITE_PAPER_PDF): $(WHITE_PAPER_deps)

# HTML.
WHITE_PAPER = white-paper
all-local: $(srcdir)/$(WHITE_PAPER).stamp

# The HTML output contains several files (the HTML file itself, its
# CSS style sheet, possibly some images).  Therefore, we cannot rely
# on a simple suffix rule to produce them.  Instead, put the products
# in a directory and use a time stamp.
$(srcdir)/$(WHITE_PAPER).stamp: $(srcdir)/white-paper.tex $(WHITE_PAPER_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(WHITE_PAPER).tmp
	mkdir $(WHITE_PAPER).tmp
## Copy images.
	mkdir $(WHITE_PAPER).tmp/figures
	cp $(WHITE_PAPER_FIGURES) $(WHITE_PAPER).tmp/figures
	$(TEXI2HTML) $(TEXI2HTML_FLAGS)			\
	  -o $(WHITE_PAPER).tmp/$(WHITE_PAPER).html $<
	rm -rf $(srcdir)/$(WHITE_PAPER)
## FIXME: moving directories between file systems is not portable.
	mv $(WHITE_PAPER).tmp $(srcdir)/$(WHITE_PAPER)
	@mv -f $@.tmp $@
EXTRA_DIST += $(srcdir)/$(WHITE_PAPER).stamp
MAINTAINERCLEANFILES += $(srcdir)/$(WHITE_PAPER).stamp

$(srcdir)/$(WHITE_PAPER): $(srcdir)/$(WHITE_PAPER).stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
DATA_html_dirs += $(WHITE_PAPER)


## ----------------- ##
## Reference Guide.  ##
## ----------------- ##

EXTRA_DIST += ref-guide.tex

REF_GUIDE_deps =				\
  $(EXAMPLES)					\
  $(SPLIT_EXAMPLES)				\
  $(OUTPUTS)					\
  $(SPLIT_OUTPUTS)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

# PDF.
REF_GUIDE_PDF = $(srcdir)/ref-guide.pdf
dist_pdf_DATA += $(REF_GUIDE_PDF)
$(REF_GUIDE_PDF): $(REF_GUIDE_deps)

# HTML.
REF_GUIDE = ref-guide
all-local: $(srcdir)/$(REF_GUIDE).stamp

# The HTML output contains several files (the HTML file itself, its
# CSS style sheet, possibly some images).  Therefore, we cannot rely
# on a simple suffix rule to produce them.  Instead, put the products
# in a directory and use a time stamp.
$(srcdir)/$(REF_GUIDE).stamp: $(srcdir)/ref-guide.tex $(REF_GUIDE_deps)
	@rm -f $@.tmp
	@touch $@.tmp
	rm -rf $(REF_GUIDE).tmp
	mkdir $(REF_GUIDE).tmp
	$(TEXI2HTML) $(TEXI2HTML_FLAGS)			\
	  -o $(REF_GUIDE).tmp/$(REF_GUIDE).html $<
	rm -rf $(srcdir)/$(REF_GUIDE)
## FIXME: moving directories between file systems is not portable.
	mv $(REF_GUIDE).tmp $(srcdir)/$(REF_GUIDE)
	@mv -f $@.tmp $@
EXTRA_DIST += $(srcdir)/$(REF_GUIDE).stamp
MAINTAINERCLEANFILES += $(srcdir)/$(REF_GUIDE).stamp

$(srcdir)/$(REF_GUIDE): $(srcdir)/$(REF_GUIDE).stamp
## Recover from the removal of $@.
	@if test -d $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
DATA_html_dirs += $(REF_GUIDE)

# Intermediate product for the (Doxygen) User Reference Manual.
REF_GUIDE_HH = $(srcdir)/ref-guide.dox
$(REF_GUIDE_HH): $(srcdir)/$(REF_GUIDE).stamp $(srcdir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(srcdir)/tools/todoxygen.sh \
	  $(srcdir)/$(REF_GUIDE)/$(REF_GUIDE).html $@.tmp $(srcdir)
	mv -f $@.tmp $@
EXTRA_DIST += $(REF_GUIDE_HH)
MAINTAINERCLEANFILES += $(REF_GUIDE_HH)


## ---------- ##
## Programs.  ##
## ---------- ##

include $(srcdir)/programs-examples.mk
EXTRA_PROGRAMS = $(PROGRAMS_examples)
CLEANFILES = $(PROGRAMS_examples)

## --------- ##
## Outputs.  ##
## --------- ##

$(srcdir)/outputs.mk: $(srcdir)/gen-make-variable
	rm -f $@.tmp
# Ignore sources in examples/trash/.
	$(srcdir)/gen-make-variable OUTPUTS				\
	  `find $(srcdir)/examples					\
               -name \*.cc -a \! -path $(srcdir)/examples/trash/\*	\
	     | sed -e 's,$(srcdir)/examples/,,'				\
	           -e 'y,/,_,'						\
	           -e 's,\(.*\)\.cc$$,$$(srcdir)/outputs/\1.txt,'`	\
	  >$@.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/outputs.mk
EXTRA_DIST += $(OUTPUTS)
MAINTAINERCLEANFILES += $(OUTPUTS)

.PHONY: regen-outputs-mk
regen-outputs-mk:
	rm -f $(srcdir)/outputs.mk.tmp
# Ignore sources in examples/trash/.
	$(srcdir)/gen-make-variable OUTPUTS				\
	  `find $(srcdir)/examples					\
               -name \*.cc -a \! -path $(srcdir)/examples/trash/\*	\
	     | sed -e 's,$(srcdir)/examples/,,'				\
	           -e 'y,/,_,'						\
	           -e 's,\(.*\)\.cc$$,$$(srcdir)/outputs/\1.txt,'`	\
	  >$(srcdir)/outputs.mk.tmp
	mv -f $(srcdir)/outputs.mk.tmp $(srcdir)/outputs.mk
	chmod -w $(srcdir)/outputs.mk

## These outputs are also required by the (LaTeX) documentation.
## However, they have no corresponding program in examples/, hence
## they cannot be regenerated.
MORE_OUTPUTS =					\
  $(srcdir)/outputs/graph-output-1.txt		\
  $(srcdir)/outputs/ima2d-display-1.txt		\
  $(srcdir)/outputs/ima2d-display-2.txt		\
  $(srcdir)/outputs/ima2d-display-output-1.txt	\
  $(srcdir)/outputs/ima2d-display-output-2.txt	\
  $(srcdir)/outputs/parray-display-1.txt	\
  $(srcdir)/outputs/win-create-1-display.txt
EXTRA_DIST += $(MORE_OUTPUTS)

# Generate a Makefile helper containing outputs rules and timestamps.
EXTRA_DIST += gen-examples-outputs-mk

$(srcdir)/examples-outputs.mk: $(srcdir)/gen-examples-outputs-mk
	rm -f $@.tmp
# Ignore sources in examples/trash/.
	cd $(srcdir) &&							\
	  ./gen-examples-outputs-mk					\
	    `find examples -name \*.cc -a \! -path examples/trash/\*	\
	       | env LC_ALL=C sort`					\
	    >examples-outputs.mk.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/examples-outputs.mk

.PHONY: regen-examples-outputs-mk
regen-examples-outputs-mk:
	rm -f $(srcdir)/examples-outputs.mk.tmp
	cd $(srcdir) &&							\
	  ./gen-examples-outputs-mk					\
	    `find examples -name \*.cc -a \! -path examples/trash/\*	\
	       | env LC_ALL=C sort`					\
	    >examples-outputs.mk.tmp
	mv -f $(srcdir)/examples-outputs.mk.tmp $(srcdir)/examples-outputs.mk
	chmod -w $(srcdir)/examples-outputs.mk

## --------------- ##
## Split outputs.  ##
## --------------- ##

# Generate a Makefile helper handling the generation of split outputs.
EXTRA_DIST += gen-split-outputs-mk
$(srcdir)/split-outputs.mk: $(srcdir)/gen-split-outputs-mk
	rm -f $@.tmp
	cd $(srcdir) && ./gen-split-outputs-mk >split-outputs.mk.tmp
	mv -f $@.tmp $@
	chmod -w $@
include $(srcdir)/split-outputs.mk
EXTRA_DIST += $(SPLIT_OUTPUTS)
MAINTAINERCLEANFILES += $(SPLIT_OUTPUTS)

.PHONY: regen-split-outputs-mk
regen-split-outputs-mk:
	rm -f $(srcdir)/split-outputs.mk.tmp
	cd $(srcdir) && ./gen-split-outputs-mk >split-outputs.mk.tmp
	mv -f $(srcdir)/split-outputs.mk.tmp $(srcdir)/split-outputs.mk
	chmod -w $(srcdir)/split-outputs.mk


## -------------- ##
## Regeneration.  ##
## -------------- ##

# Regen Make helpers.
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	$(MAKE) $(AM_MAKEFLAGS) regen-figures-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-split-examples-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-outputs-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-outputs-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-split-outputs-mk


## -------------- ##
## Distribution.  ##
## -------------- ##

EXTRA_DIST +=					\
  $(DATA_html_dirs)				\
  $(DOXYFILE).in				\
  mln/win/win.dox				\
  mln/main.dox					\
  mln/morpho/morpho.dox				\
  mln/canvas/canvas.dox				\
  mln/data/data.dox				\
  mln/util/graph.dox				\
  mln/util/util.dox				\
  mln/io/io.dox					\
  mln/accu/accu.dox				\
  mln/core/image/morphers.dox			\
  mln/core/image/identity_morpher.dox		\
  mln/core/image/images.dox			\
  mln/core/image/domain_morpher.dox		\
  mln/core/image/value_morpher.dox		\
  mln/core/image/concrete.dox			\
  mln/core/neighb.dox				\
  mln/core/site_set/site_set.dox		\
  img/small-enlarged.png			\
  img/small.png					\
  doxyfuns.sty					\
  milena.sty					\
  tools/sample_utils.hh				\
  tools/split_sample.sh				\
  tools/todoxygen.sh				\
  mainpage.dox


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

## FIXME: The current HTML outputs of technical.tex, tutorial.tex and
## ref-guide.tex are awful.  Only install the white paper HTML product
## for the moment.
# DATA_html_dirs_inst = $(DATA_html_dirs)
DATA_html_dirs_inst = $(WHITE_PAPER)

# We install by hand, otherwise Automake produces "install .../srcdoc
# .../srcdoc", which installs our dir into the previous one, instead
# of replacing it.
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(htmldir)/milena/
# The user reference manual is located in a subdirectory, hence this
# special case.
	rm -rf $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN)/html
	cp -r $(srcdir)/$(USER_REFMAN)/html $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN)
## Fix path to the main page of the documentation.
	chmod -R u+w $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN)
	find  $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN) -name '*.html' -exec \
	  perl -pi							      \
	    -e "s,../../../../doc/$(USER_REFMAN)/html,../../$(USER_REFMAN),g" \
	    {} \;
# Other HTML directories can be processed uniformly.
	for d in $(DATA_html_dirs_inst); do			 \
	  rm -rf $(DESTDIR)$(htmldir)/milena/$$d;	 \
	  cp -r $(srcdir)/$$d $(DESTDIR)$(htmldir)/milena/;\
	done
## Try GNU chmod's X mode before resorting to something slower but
## more standard.
	chmod -R a+rX $(DESTDIR)$(htmldir)/milena/ ||	\
	  find $(DESTDIR)$(htmldir)/milena		\
	       \( -type d -a -exec chmod a+rx {} \; \)	\
	       -o -exec chmod a+r {} \;

uninstall-local:
	for d in $(DATA_html_dirs_inst); do			\
	  chmod -R 700 $(DESTDIR)$(htmldir)/milena/$$d;	\
	  rm -rf $(DESTDIR)$(htmldir)/milena/$$d;	\
	done
# Same remark as above regarding the special case of the user
# reference manual.
	chmod -R 700 $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN)
	rm -rf $(DESTDIR)$(htmldir)/milena/$(USER_REFMAN)
	rm -rf $(DESTDIR)$(htmldir)/milena


doc-teamcity:
	find  $(srcdir)/$(USER_REFMAN) -name '*.html' -exec		     \
	  perl -pi -e "s,../../../../doc/$(USER_REFMAN)/html,../olena.doc,g" \
	    {} \;
