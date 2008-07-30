## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  box1d						\
  box2d						\
  box3d						\
						\
  cast_image					\
  category					\
  clock_neighb2d				\
  clock_test					\
  clone						\
  complex					\
						\
  decorated_image				\
  dpoint1d					\
  dpoint2d					\
  dpoint3d					\
  dpoints_pixter				\
						\
  exact						\
						\
  h_vec						\
  hexa						\
						\
  image1d					\
  image2d					\
  image2d_h					\
  image3d					\
  image_if					\
  image_if_interval				\
  image_if_value				\
  initialize					\
  interpolated					\
						\
  graph_elt_neighborhood			\
  graph_elt_window				\
  graph_image					\
						\
  line_graph_elt_neighborhood			\
  line_graph_elt_window				\
  line_graph_image				\
  line_piter					\
						\
  mono_obased_rle_image				\
  mono_rle_image				\
						\
  neighb					\
						\
  obased_rle_image				\
						\
  p_priority_queue				\
  p_priority_queue_fast				\
  p_priority_queue_fast_with_array		\
  p_queue					\
  p_queue_fast					\
  p_runs					\
  p_set						\
  pixel						\
  pixter1d					\
  pixter1d_more					\
  pixter2d					\
  pixter2d_more					\
  pixter3d					\
  pixter3d_more					\
  plain						\
  point1d					\
  point2d					\
  point3d					\
  point_set_compatibility			\
  pset_array					\
  pset_if					\
						\
  rle_image					\
						\
  safe_image					\
  sparse_image					\
  sub_image					\
						\
  t_image					\
  tr_image					\
  translate_image				\
						\
  window1d					\
  window2d					\
  window3d					\
  w_window1d_int				\
  w_window2d_int				\
  w_window3d_int

box1d_SOURCES = box1d.cc
box2d_SOURCES = box2d.cc
box3d_SOURCES = box3d.cc

cast_image_SOURCES = cast_image.cc
category_SOURCES = category.cc
clock_neighb2d_SOURCES = clock_neighb2d.cc
clock_test_SOURCES = clock_test.cc
clone_SOURCES = clone.cc
complex_SOURCES = complex.cc

decorated_image_SOURCES = decorated_image.cc
dpoint1d_SOURCES = dpoint1d.cc
dpoint2d_SOURCES = dpoint2d.cc
dpoint3d_SOURCES = dpoint3d.cc
dpoints_pixter_SOURCES = dpoints_pixter.cc

exact_SOURCES = exact.cc

h_vec_SOURCES = h_vec.cc
hexa_SOURCES = hexa.cc

image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image2d_h_SOURCES = image2d_h.cc
image3d_SOURCES = image3d.cc
image_if_SOURCES = image_if.cc
image_if_interval_SOURCES = image_if_interval.cc
image_if_value_SOURCES = image_if_value.cc
initialize_SOURCES = initialize.cc
interpolated_SOURCES = interpolated.cc

graph_elt_neighborhood_SOURCES = graph_elt_neighborhood.cc
graph_elt_window_SOURCES = graph_elt_window.cc
graph_image_SOURCES = graph_image.cc

line_graph_elt_neighborhood_SOURCES = line_graph_elt_neighborhood.cc
line_graph_elt_window_SOURCES = line_graph_elt_window.cc
line_graph_image_SOURCES = line_graph_image.cc
line_piter_SOURCES = line_piter.cc

mono_obased_rle_image_SOURCES = mono_obased_rle_image.cc
mono_rle_image_SOURCES = mono_rle_image.cc

neighb_SOURCES = neighb.cc

obased_rle_image_SOURCES = obased_rle_image.cc

p_priority_queue_SOURCES = p_priority_queue.cc
p_priority_queue_fast_SOURCES = p_priority_queue_fast.cc
p_priority_queue_fast_with_array_SOURCES = p_priority_queue_fast.cc
p_queue_SOURCES = p_priority_queue_fast.cc
p_queue_fast_SOURCES = p_priority_queue_fast.cc
p_runs_SOURCES = p_runs.cc
p_set_SOURCES = p_set.cc
pixel_SOURCES = pixel.cc
pixter1d_SOURCES = pixter1d.cc
pixter1d_more_SOURCES = pixter1d_more.cc
pixter2d_SOURCES = pixter2d.cc
pixter2d_more_SOURCES = pixter2d_more.cc
pixter3d_SOURCES = pixter3d.cc
pixter3d_more_SOURCES = pixter3d_more.cc
plain_SOURCES = plain.cc
point1d_SOURCES = point1d.cc
point2d_SOURCES = point2d.cc
point3d_SOURCES = point3d.cc
point_set_compatibility_SOURCES = point_set_compatibility.cc
pset_array_SOURCES = pset_array.cc
pset_if_SOURCES = pset_if.cc

rle_image_SOURCES = rle_image.cc

safe_image_SOURCES = safe_image.cc
sparse_image_SOURCES = sparse_image.cc
sub_image_SOURCES = sub_image.cc

t_image_SOURCES = t_image.cc
tr_image_SOURCES = tr_image.cc
translate_image_SOURCES = translate_image.cc

window1d_SOURCES = window1d.cc
window2d_SOURCES = window2d.cc
window3d_SOURCES = window3d.cc
w_window1d_int_SOURCES = w_window1d_int.cc
w_window2d_int_SOURCES = w_window2d_int.cc
w_window3d_int_SOURCES = w_window3d_int.cc

# Tests depending on the FreeImagePlus library.
if FREEIMAGEPLUS
  check_PROGRAMS += fi_adaptor
  fi_adaptor_SOURCES = fi_adaptor.cc
  fi_adaptor_CPPFLAGS = $(AM_CPPFLAGS) $(FREEIMAGEPLUS_CPPFLAGS)
  fi_adaptor_LDFLAGS = $(AM_LDFLAGS) $(FREEIMAGEPLUS_LDFLAGS)
endif

TESTS = $(check_PROGRAMS)
