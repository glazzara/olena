## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/doc/doc.mk

bin_PROGRAMS =			\
  borderthickness 		\
  dpoint-1 			\
  graph-data-full 		\
  ima-has 			\
  ima-size 			\
  ima2d-1 			\
  ima2d-decl-2-blobs-full 	\
  ima2d-rot 			\
  labeling-compute-full 	\
  logical-not 			\
  paste-call-1 			\
  point-1

borderthickness_SOURCES = borderthickness.cc
dpoint_1_SOURCES = dpoint-1.cc
graph_data_full_SOURCES = graph-data-full.cc
ima_has_SOURCES = ima-has.cc
ima_size_SOURCES = ima-size.cc
ima2d_1_SOURCES = ima2d-1.cc
ima2d_decl_2_blobs_full_SOURCES = ima2d-decl-2-blobs-full.cc
ima2d_rot_SOURCES = ima2d-rot.cc
labeling_compute_full_SOURCES = labeling-compute-full.cc
logical_not_SOURCES = logical-not.cc
paste_call_1_SOURCES = paste-call-1.cc
point_1_SOURCES = point-1.cc

all: clean-png-figures
	mkdir -p $(OUTPUTS_BUILDDIR) 
	mkdir -p $(FIGURES_BUILDDIR) 
	@failcom='exit 1'; 						\
	list='$(bin_PROGRAMS)'; for bin in $$list; do 			\
	  echo "Running $$bin"; 					\
	  ./$$bin > $(OUTPUTS_BUILDDIR)/$$bin.txt || eval $$failcom; 	\
	done;								\
 	has_txt_diff="";						\
 	txtlist="$(OUTPUTS_BUILDDIR)/*"; for txt in $$txtlist; do 	\
 	  echo "Diff `basename $$txt`";					\
 	  diff $$txt $(OUTPUTS_SRCDIR)/`basename $$txt` >/dev/null 2>&1	\
 		|| has_txt_diff+=" `basename $$txt`";	\
 	done;								\
 	has_fig_diff="";						\
 	figlist="$(FIGURES_BUILDDIR)/*"; for fig in $$figlist; do 	\
 	  echo "Diff `basename $$fig`";					\
 	  diff $$fig $(FIGURES_SRCDIR)/`basename $$fig` >/dev/null 2>&1 \
 		|| has_fig_diff=" `basename $$fig`";	\
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
  	     echo "* Figures:"; 						\
  	     echo "";							\
  	     echo "$$has_fig_diff"; 					\
  	     echo "";							\
	     echo "--------"; 						\
  	     echo "* Please update the reference files located in:";	\
  	     echo "";							\
	     echo " $(OUTPUTS_SRCDIR)"; 					\
	     echo " $(FIGURES_SRCDIR)"; 					\
  	     echo "";							\
  	     eval $$failcom);


clean-local:
	rm -Rf $(OUTPUTS_BUILDDIR) $(FIGURES_BUILDDIR)

## Clean up converted images. (*.p*m -> *.png)
clean-png-figures:
	rm -f $(FIGURES_BUILDDIR)/*.png
