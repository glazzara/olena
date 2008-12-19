## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/doc/doc.mk

noinst_PROGRAMS =		\
  accu-right-instanciation 	\
  borderthickness 		\
  box2d-bbox 			\
  domain-display 		\
  dpoint-1 			\
  estim-sites 			\
  extension-ignore 		\
  fill 				\
  fill-call-1			\
  fill-part-image 		\
  fill-subdomain 		\
  fill-subdomain-shorter	\
  fill-subimage-cfun 		\
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
  ima-size 			\
  labeling-compute 		\
  logical-not 			\
  mln_var 			\
  parray-bbox 			\
  paste 			\
  paste-call-1 			\
  parray-append 		\
  point-1 			\
  predicate-1 			\
  win-create-1 			\
  win-create-2

noinst_PROGRAMS += \
tuto1_first_image

accu_right_instanciation_SOURCES = accu-right-instanciation.cc
borderthickness_SOURCES = borderthickness.cc
box2d_bbox_SOURCES = box2d-bbox.cc
domain_display_SOURCES = domain-display.cc
dpoint_1_SOURCES = dpoint-1.cc
estim_sites_SOURCES = estim-sites.cc
extension_ignore_SOURCES = extension-ignore.cc
fill_SOURCES = fill.cc
fill_call_1_SOURCES = fill-call-1.cc
fill_part_image_SOURCES = fill-part-image.cc
fill_subdomain_SOURCES = fill-subdomain.cc
fill_subdomain_shorter_SOURCES = fill-subdomain-shorter.cc
fill_subimage_cfun_SOURCES = fill-subimage-cfun.cc
forall_piter_SOURCES = forall-piter.cc
fun_p2v_1_SOURCES = fun-p2v-1.cc
graph_data_SOURCES = graph-data.cc
graph_iter_SOURCES = graph-iter.cc
ima_has_SOURCES = ima-has.cc
ima_save_SOURCES = ima-save.cc
ima_size_SOURCES = ima-size.cc
ima2d_1_SOURCES = ima2d-1.cc
ima2d_2_SOURCES = ima2d-2.cc
ima2d_3_SOURCES = ima2d-3.cc
ima2d_4_SOURCES = ima2d-4.cc
ima2d_5_SOURCES = ima2d-5.cc
ima2d_6_clone_SOURCES = ima2d-6-clone.cc
ima2d_7_SOURCES = ima2d-7.cc
ima2d_rot_SOURCES = ima2d-rot.cc
labeling_compute_SOURCES = labeling-compute.cc
logical_not_SOURCES = logical-not.cc
mln_var_SOURCES = mln_var.cc
paste_SOURCES = paste.cc
paste_call_1_SOURCES = paste-call-1.cc
parray_append_SOURCES = parray-append.cc
parray_bbox_SOURCES = parray-bbox.cc
point_1_SOURCES = point-1.cc
predicate_1_SOURCES = predicate-1.cc
win_create_1_SOURCES = win-create-1.cc
win_create_2_SOURCES = win-create-2.cc

tuto1_first_image_SOURCES = tuto1_first_image.cc

all: clean-figures 
	mkdir -p $(OUTPUTS_BUILDDIR) 
	mkdir -p $(FIGURES_BUILDDIR) 


run-samples: all
	@failcom='exit 1'; 						\
	list='$(noinst_PROGRAMS)'; for bin in $$list; do 		\
	  echo "Running $$bin"; 					\
	  ./$$bin > $(OUTPUTS_BUILDDIR)/$$bin.txt || eval $$failcom; 	\
	done


data: run-samples diff-data split-samples split-outputs 


diff-data:
	@failcom='exit 1'; 						\
 	has_txt_diff="";						\
 	txtlist="$(OUTPUTS_BUILDDIR)/*.txt"; for txt in $$txtlist; do 	\
 	  echo "Diff `basename $$txt`";					\
 	  diff $$txt $(OUTPUTS_SRCDIR)/`basename $$txt` >/dev/null 2>&1	\
 		|| has_txt_diff+=" `basename $$txt`";	\
 	done;								\
 	has_fig_diff="";						\
 	figlist="$(FIGURES_BUILDDIR)/*"; for fig in $$figlist; do 	\
 	  echo "Diff `basename $$fig`";					\
 	  diff $$fig $(FIGURES_SRCDIR)/`basename $$fig` >/dev/null 2>&1 \
 		|| has_fig_diff+=" `basename $$fig`";	\
 	done; 								\
	(test -z "$$has_txt_diff" && test -z "$$has_fig_diff")		\
	 || (echo "--------";						\
	     echo "ERROR: please check if the tutorial needs an update.";\
	     echo "       The following files differ from their \
reference or their reference file does not exist:";  			\
	     echo "--------"; 						\
  	     echo "* Outputs:";						\
  	     echo "";							\
  	     echo "$$has_txt_diff"; 					\
  	     echo "";							\
	     echo "--------"; 						\
  	     echo "* Figures:"; 					\
  	     echo "";							\
  	     echo "$$has_fig_diff"; 					\
  	     echo "";							\
	     echo "--------"; 						\
  	     echo "* Please update manually the reference files located \
in:";									\
  	     echo "";							\
	     echo " $(OUTPUTS_SRCDIR)"; 				\
	     echo " $(FIGURES_SRCDIR)"; 				\
  	     echo "";							\
	     echo "* Reference files can updated automatically with \
'make fix-refdata'"; 							\
  	     echo "";							\
  	     echo "";							\
  	     eval $$failcom);


fix-refdata:
	@failcom='exit 1'; 						\
 	txtlist="$(OUTPUTS_BUILDDIR)/*.txt"; for txt in $$txtlist; do 	\
 	  diff $$txt $(OUTPUTS_SRCDIR)/`basename $$txt` >/dev/null 2>&1	\
 		|| echo "Updating reference file for $$txt"; 		\
		   cp $$txt $(OUTPUTS_SRCDIR);				\
 	done;								\
 	figlist="$(FIGURES_BUILDDIR)/*.p*m"; for fig in $$figlist; do 	\
 	  diff $$fig $(FIGURES_SRCDIR)/`basename $$fig` >/dev/null 2>&1 \
 		|| echo "Updating reference file for $$fig";		\
		   cp $$fig $(FIGURES_SRCDIR);				\
 	done; 								


split-samples:
	mkdir -p $(SAMPLES_BUILDDIR)
	@failcom='exit 1'; 						\
	list='$(SAMPLES_SRCDIR)/*.cc'; for file in $$list; do 		\
	$(TUTORIAL_SRCDIR)/tools/split_sample.sh $$file cc $(SAMPLES_BUILDDIR)\
					      || eval $$failcom; 	\
	done

split-outputs:
	mkdir -p $(SPLITTED_OUTPUTS_BUILDDIR)
	@failcom='exit 1'; 						\
	list='$(OUTPUTS_BUILDDIR)/*.txt'; for file in $$list; do 	\
	$(TUTORIAL_SRCDIR)/tools/split_sample.sh $$file txt $(SPLITTED_OUTPUTS_BUILDDIR)\
					      || eval $$failcom; 	\
	done


clean-local: clean-figures
	rm -Rf $(OUTPUTS_BUILDDIR)
	rm -f $(SAMPLES_BUILDDIR)/*.cc

clean-figures:
	rm -Rf $(FIGURES_BUILDDIR)
