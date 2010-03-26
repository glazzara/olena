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

DOXYGEN = doxygen

DOXYFILE = Doxyfile


## --------------------- ##
## LaTeX configuration.  ##
## --------------------- ##

include $(top_srcdir)/build-aux/tex.mk

# FIXME: Check this list.
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
  $(PPM_FIGURES:.ppm=.png)
EXTRA_DIST += $(PNG_FIGURES)
MAINTAINERCLEANFILES += $(PNG_FIGURES)

# --------------- #
# Other figures.  #
# --------------- #

# This image seems to be used nowhere.
MORE_FIGURES = figures/accu_image_morpher_big_picture.pdf
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

refman_dependencies =				\
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

USER_REFMAN_PDF = user-refman.pdf
USER_REFMAN = user-refman

$(srcdir)/$(USER_REFMAN_PDF): $(srcdir)/$(USER_REFMAN)
	cd $(srcdir)/$(USER_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(USER_REFMAN)/latex/refman.pdf $@

EXTRA_DIST += $(srcdir)/user-refman.stamp
MAINTAINERCLEANFILES += $(srcdir)/user-refman.stamp
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

DEVEL_REFMAN_PDF = devel-refman.pdf
DEVEL_REFMAN = devel-refman

# Be warned, most LaTeX installations will not be able to compile the
# PDF development documentation as-is, as TeX's default memory
# capacities are usually too small to handle such a large document.
$(srcdir)/$(DEVEL_REFMAN_PDF): $(srcdir)/$(DEVEL_REFMAN)
	cd $(srcdir)/$(DEVEL_REFMAN)/latex && $(MAKE) $(AM_MAKEFLAGS) pdf
	cp -f $(srcdir)/$(DEVEL_REFMAN)/latex/refman.pdf $@

MAINTAINERCLEANFILES += $(srcdir)/devel-refman.stamp
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


## ------------------------- ##
## Technical Documentation.  ##
## ------------------------- ##

technical_dir = $(srcdir)/technical

technical_dependencies =			\
  $(TECHNICAL_EXAMPLES)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

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

dist_html_DATA = $(TECHNICAL_HTML_OUTPUTS)

# Final product.
TECHNICAL_PDF = $(technical_dir)/technical.pdf
# Extra dependencies.
$(TECHNICAL_PDF): $(technical_dependencies)

dist_pdf_DATA = $(TECHNICAL_PDF)

EXTRA_DIST += $(technical_dir)/technical.tex


## ---------- ##
## Tutorial.  ##
## ---------- ##

tutorial_dir = $(srcdir)/tutorial

tutorial_dependencies =				\
  $(EXAMPLES)					\
  $(SPLIT_EXAMPLES)				\
  $(OUTPUTS)					\
  $(SPLIT_OUTPUTS)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

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

dist_html_DATA += $(TUTORIAL_HTML_OUTPUTS)

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: TUTORIAL_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
TUTORIAL_HH = $(tutorial_dir)/tutorial.hh
$(TUTORIAL_HH): $(TUTORIAL_HTML) $(srcdir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(srcdir)/tools/todoxygen.sh $< $@.tmp $(srcdir)
	mv -f $@.tmp $@


# Final product.
TUTORIAL_PDF = $(tutorial_dir)/tutorial.pdf
# Extra dependencies.
$(TUTORIAL_PDF): $(tutorial_dependencies)

dist_pdf_DATA += $(TUTORIAL_PDF)

EXTRA_DIST +=					\
  $(tutorial_dir)/tutorial.tex			\
  $(TUTORIAL_HH)

MAINTAINERCLEANFILES += $(TUTORIAL_HH)


## ------------- ##
## White Paper.  ##
## ------------- ##

white_paper_dir = $(srcdir)/white_paper

PNGS =								\
  $(white_paper_dir)/figures/house.png				\
  $(white_paper_dir)/figures/house_rag.png			\
  $(white_paper_dir)/figures/house_wshed.png			\
  $(white_paper_dir)/figures/house_wshed_mean_colors.png

EPSS = $(PNGS:png=eps)

.png.eps:
	convert $< $@

# Final product.
WHITE_PAPER_HTML = $(white_paper_dir)/white_paper.html
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

dist_html_DATA += $(WHITE_PAPER_HTML_OUTPUTS)

WHITE_PAPER_PDF = $(white_paper_dir)/white_paper.pdf
# Extra dependencies.
$(WHITE_PAPER_PDF): $(PNGS)

dist_pdf_DATA += $(WHITE_PAPER_PDF)

EXTRA_DIST +=					\
  $(white_paper_dir)/white_paper.tex		\
  $(PNGS)					\
  $(EPSS)

MAINTAINERCLEANFILES += $(EPSS)


## ----------------- ##
## Reference Guide.  ##
## ----------------- ##

ref_guide_dir = $(srcdir)/ref_guide

ref_guide_dependencies =			\
  $(EXAMPLES)					\
  $(SPLIT_EXAMPLES)				\
  $(OUTPUTS)					\
  $(SPLIT_OUTPUTS)				\
  $(PNG_FIGURES)				\
  $(DOC_PACKAGES)

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

dist_html_DATA += $(REF_GUIDE_HTML_OUTPUTS)

# Intermediate product for the various doc targets of the parent
# directory.
#
# This is not a bug: REF_GUIDE_HH is meant to have a `.hh' extension,
# since it is later parsed by Doxygen, which complains about `.html'
# files.
REF_GUIDE_HH = $(ref_guide_dir)/ref_guide.hh
$(REF_GUIDE_HH): $(REF_GUIDE_HTML) $(srcdir)/tools/todoxygen.sh
# The script `todoxygen.sh' may fail and still create a (partial and
# invalid) file, thus preventing Make from trying to generate it
# again.  The solution is to use a temporary file as output and rename
# it if the script succeeds.
	rm -f $@.tmp
	$(srcdir)/tools/todoxygen.sh $< $@.tmp $(srcdir)
	mv -f $@.tmp $@


# Final product.
REF_GUIDE_PDF = $(ref_guide_dir)/ref_guide.pdf
# Extra dependencies.
$(REF_GUIDE_PDF): $(ref_guide_dependencies)

dist_pdf_DATA += $(REF_GUIDE_PDF)

EXTRA_DIST +=					\
  $(ref_guide_dir)/ref_guide.tex		\
  $(REF_GUIDE_HH)

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
EXTRA_DIST += generate_dist_files.sh
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	$(MAKE) $(AM_MAKEFLAGS) regen-figures-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-split-examples-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-outputs-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-examples-outputs-mk
	$(MAKE) $(AM_MAKEFLAGS) regen-split-outputs-mk


## -------------- ##
## Installation.  ##
## -------------- ##

# PDF.
dist_pdf_DATA += $(USER_REFMAN_PDF)

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


MAINTAINERCLEANFILES += $(dist_html_DATA) $(dist_pdf_DATA)


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
