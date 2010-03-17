## Program as examples, generating images & outputs.  -*- Makefile-Automake -*-

## Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

## These variable are set aside in their own file, so that the
## generation of examples-outputs.mk depends only on this file, instead
## of Makefile.am.

## FIXME: Generate this file?  Probably a good idea.
##
## If generated, merge examples-outputs.mk and outputs.mk?

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
  forall-piter  		\
  fun-p2v-1 			\
  graph-data 			\
  graph-iter 			\
  ima-has 			\
  ima-load			\
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

PROGRAMS_examples +=				\
  tuto2_first_image				\
  tuto3_rw_image				\
  tuto4_genericity_and_algorithms

# Tuto3
PROGRAMS_examples +=				\
  tuto3_colorize				\
  tuto3_first_routine				\
  tuto3_println					\
  tuto3_trace

# Tuto4
PROGRAMS_examples +=				\
  tuto4_image					\
  tuto4_point2d					\
  tuto4_site_set_create

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
ima_load_SOURCES = examples/ima-load.cc
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
tuto3_first_routine_SOURCES = examples/tuto3/first_routine.cc
tuto3_println_SOURCES = examples/tuto3/println.cc
tuto3_trace_SOURCES = examples/tuto3/trace.cc

# Tuto 4
tuto4_image_SOURCES = examples/tuto4/image.cc
tuto4_point2d_SOURCES = examples/tuto4/point2d.cc
tuto4_site_set_create_SOURCES = examples/tuto4/site_set_create.cc
