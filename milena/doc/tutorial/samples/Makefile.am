## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/doc/doc.mk

bin_PROGRAMS =			\
  borderthickness 		\
  dpoint-1 			\
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
ima_has_SOURCES = ima-has.cc
ima_size_SOURCES = ima-size.cc
ima2d_1_SOURCES = ima2d-1.cc
ima2d_decl_2_blobs_full_SOURCES = ima2d-decl-2-blobs-full.cc
ima2d_rot_SOURCES = ima2d-rot.cc
labeling_compute_full_SOURCES = labeling-compute-full.cc
logical_not_SOURCES = logical-not.cc
paste_call_1_SOURCES = paste-call-1.cc
point_1_SOURCES = point-1.cc

all:
	mkdir -p $(OUTPUTS_DIR) 
	mkdir -p $(FIGURES_DIR) 
	@failcom='exit 1'; 					\
	list='$(bin_PROGRAMS)'; for bin in $$list; do 		\
	  echo "Running $$bin"; 				\
	  ./$$bin > $(OUTPUTS_DIR)/$$bin.txt || eval $$failcom; \
	done; 

clean-local:
	rm -Rf $(OUTPUTS_DIR) $(FIGURES_DIR)

