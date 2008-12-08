SUBDIRS = samples

.PHONY: tuto tuto_html

include $(top_srcdir)/milena/doc/doc.mk
TEXINPUTS = $(TUTORIAL_SRCDIR):$(TUTORIAL_BUILDDIR):$(OUTPUTS_SRCDIR):

tuto: data fig-convert
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
	  convert $$img $(FIGURES_BUILDDIR)/`basename $$img $$ext`png		\
	    || eval $$failcom; 							\
	done; 

clean-local:
	rm -f tutorial.aux tutorial.toc tutorial.log tutorial.bbl tutorial.out
	rm -f *~ *blg *.lot
	rm -f tutorial.pdf
	rm -f *.haux *.hh *.html *.htoc

EXTRA_DIST = tutorial.tex

EXTRA_DIST += \
examples/cpp_issue.cc \
examples/sub_image.cc \
examples/sub_image_if.cc \
examples/p_vaccess.cc \
examples/win_multiple.cc \
examples/image2d.cc \
examples/image_plain.cc \
examples/p_run.cc \
examples/proxy.cc \
examples/p_array.2.cc \
examples/for_Z.cc \
examples/image_if.cc \
examples/p_key.cc \
examples/p_mutable_array_of.cc \
examples/p_queue.cc \
examples/image_flat.cc \
examples/p_image.cc \
examples/box.cc \
examples/p_line2d.cc \
examples/p_array.cc \
examples/accu.cc \
examples/p_key.2.cc \
examples/window.cc \
examples/image_safe.cc \
examples/vec.cc

EXTRA_DIST += \
figures/ima2d-rot-2.ppm \
figures/logical-not-2.pbm \
figures/fill-subdomain-2.ppm \
figures/labeling-compute-2.ppm \
figures/ima2d-rot-1.ppm \
figures/fill-subdomain-3.ppm \
figures/logical-not-1.pbm \
figures/ima_save.pbm \
figures/fill-subimage-cfun-1.ppm \
figures/fill-subdomain-4.ppm \
figures/logical-not-3.pbm \
figures/labeling-compute-1.pbm \
figures/fill-subdomain-1.pbm

EXTRA_DIST += \
outputs/win-create-1-display.txt \
outputs/win-create-2.txt \
outputs/accu-right-instanciation.txt \
outputs/ima2d-1.txt \
outputs/borderthickness.txt \
outputs/ima-save.txt \
outputs/fill-subimage-cfun.txt \
outputs/labeling-compute.txt \
outputs/parray-display-1.txt \
outputs/fill-subdomain-shorter.txt \
outputs/dpoint-1.txt \
outputs/forall-piter.txt \
outputs/predicate-1.txt \
outputs/box2d-bbox.txt \
outputs/graph-iter.txt \
outputs/graph-output-1.txt \
outputs/fill-call-1.txt \
outputs/ima2d-rot.txt \
outputs/paste-call-1.txt \
outputs/fun-p2v-1.txt \
outputs/estim-sites.txt \
outputs/parray-append.txt \
outputs/fill-subdomain.txt \
outputs/ima-size.txt \
outputs/logical-not.txt \
outputs/ima2d-display-output-2.txt \
outputs/ima2d-decl-2.txt \
outputs/paste.txt \
outputs/ima2d-6-clone.txt \
outputs/ima2d-7.txt \
outputs/parray-bbox.txt \
outputs/domain-display.txt \
outputs/ima2d-display-1.txt \
outputs/win-create-1.txt \
outputs/ima2d-display-2.txt \
outputs/ima2d-display-output-1.txt \
outputs/fill-part-image.txt \
outputs/ima2d-3-output.txt \
outputs/ima-has.txt \
outputs/ima2d-5.txt \
outputs/ima2d-4.txt \
outputs/mln_var.txt \
outputs/fill.txt \
outputs/point-1.txt \
outputs/extension-ignore.txt \
outputs/graph-data.txt \
outputs/ima2d-2.txt \
outputs/ima2d-3.txt
 
EXTRA_DIST += \
samples/paste-call-1.cc \
samples/dpoint-1.cc \
samples/parray-append.cc \
samples/ima-load.cc.raw \
samples/ima2d-restricted-1.cc.raw \
samples/graph-iter.cc \
samples/labeling-compute.cc \
samples/ima2d-7.cc \
samples/ima2d-restricted-2.cc.raw \
samples/fill-subimage-cfun.cc \
samples/ima-save.cc \
samples/point-1.cc \
samples/box2d-bbox.cc \
samples/forall-piter.cc \
samples/ima-size.cc \
samples/borderthickness.cc \
samples/Makefile.in \
samples/ima-has.cc \
samples/win-create-1.cc \
samples/accu-wrong-instanciation.cc.raw \
samples/accu-right-instanciation.cc \
samples/ima2d-5.cc \
samples/fill-subdomain.cc \
samples/fill-subdomain-shorter.cc \
samples/ima2d-2.cc \
samples/extension-ignore.cc \
samples/Makefile.am \
samples/parray-bbox.cc \
samples/graph-data.cc \
samples/mln_var.cc \
samples/ima2d-4.cc \
samples/predicate-1.cc \
samples/ima2d-3.cc \
samples/ima2d-1.cc \
samples/estim-sites.cc \
samples/ima2d-6-clone.cc \
samples/ima2d-restricted-3.cc.raw \
samples/logical-not.cc \
samples/fun-p2v-1.cc \
samples/ima2d-rot.cc \
samples/fill-part-image.cc \
samples/domain-display.cc \
samples/paste.cc \
samples/fill.cc \
samples/fill-call-1.cc \
samples/win-create-2.cc

EXTRA_DIST += 		\
tools/sample_utils.hh 	\
tools/split_sample.sh 	\
tools/todoxygen.sh

