#							-*- Automake -*-

# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).
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

## FIXME: Do not forget to turn the dependency
## $(srcdir)/examples/make.mk into $(srcdir)/Makefile.am when the
## former is merged into the latter.
$(DOC_SRCDIR)/examples-outputs.mk: $(DOC_SRCDIR)/gen-examples-outputs-mk $(srcdir)/examples/make.mk
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
