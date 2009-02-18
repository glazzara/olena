SUBDIRS = samples

.PHONY: tutorial tuto_html

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(TUTORIAL_SRCDIR):$(TUTORIAL_BUILDDIR):$(OUTPUTS_SRCDIR):

tutorial: data fig-convert
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(TUTORIAL_SRCDIR)/tutorial.tex

tuto_html: data fig-convert 
	chmod u+x $(TUTORIAL_SRCDIR)/tools/todoxygen.sh
	$(TUTORIAL_SRCDIR)/tools/todoxygen.sh $(TUTORIAL_SRCDIR)/tutorial.tex $(TUTORIAL_BUILDDIR)

data:
	make -C samples data

fix-refdata:
	make -C samples fix-refdata

fig-convert:
	@failcom='exit 1'; 							\
	list="$(FIGURES_BUILDDIR)/*.p*m"; for img in $$list; do 		\
	  echo "Converting $$img to png"; 					\
	  name=`basename $$img` 						\
	  ext=`echo $$name | cut -d '.' -f 2`; 					\
	  convert -scale 250 $$img $(FIGURES_BUILDDIR)/`basename $$img $$ext`png\
	    || eval $$failcom; 							\
	done; 

clean-local:
	rm -f tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out
	rm -f *~ *blg *.lot
	rm -f tutorial.pdf
	rm -f *.haux *.hh *.html *.htoc

EXTRA_DIST = 		\
tutorial.tex 		\
tutorial.pdf

.PHONY: regen-dist
regen-dist: $(top_srcdir)/headers.stamp

$(top_srcdir)/milena/doc/tutorial/examples/examples.mk: $(top_srcdir)/headers.stamp
$(top_srcdir)/milena/doc/tutorial/figures/figures.mk: $(top_srcdir)/headers.stamp
$(top_srcdir)/milena/doc/tutorial/outputs/outputs.mk: $(top_srcdir)/headers.stamp
$(top_srcdir)/milena/doc/tutorial/samples/samples.mk: $(top_srcdir)/headers.stamp

EXTRA_DIST += $(top_srcdir)/headers.stamp
$(top_srcdir)/headers.stamp: $(top_srcdir)/milena/doc/tutorial/generate_dist_files.sh
	@rm -f $@.tmp
	@touch $@.tmp
	cd $(srcdir) && ./generate_dist_files.sh
	@mv -f $@.tmp $@

include $(top_srcdir)/milena/doc/tutorial/examples/examples.mk
include $(top_srcdir)/milena/doc/tutorial/figures/figures.mk
include $(top_srcdir)/milena/doc/tutorial/outputs/outputs.mk
include $(top_srcdir)/milena/doc/tutorial/samples/samples.mk

EXTRA_DIST += 		\
tools/sample_utils.hh 	\
tools/split_sample.sh 	\
tools/todoxygen.sh

