## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = norm					\
	  level					\
	  arith					\
	  logical				\
	  border

check_PROGRAMS =				\
  accu_all					\
  accu_min					\
  accu_nil					\
						\
  border_get					\
  border_resize_image1d_1			\
  border_resize_image1d_2			\
  border_resize_image1d_3			\
  border_resize_image2d_1			\
  border_resize_image2d_2			\
  border_resize_image2d_3			\
  border_resize_image3d_1			\
  border_resize_image3d_2			\
  border_resize_image3d_3			\
  border_resize_image_if			\
  border_resize_sub_image			\
  box1d						\
  box2d						\
  box3d						\
						\
  canvas_browsing_fwd				\
  cast_image					\
  chamfer					\
  color_pretty					\
  convert_to_image				\
  convert_to_tiles				\
  convert_to_vec_p				\
  core_category					\
  core_clone					\
  core_exact					\
  core_initialize				\
						\
  debug_println					\
  debug_println_with_border			\
  decorated_image				\
  dpoint1d					\
  dpoint2d					\
  dpoint3d					\
  dpoints_pixter				\
						\
  estim_mean					\
						\
  fi_adaptor					\
  fun_x2x_composed				\
  fun_x2x_rotation				\
  fun_x2x_translation				\
						\
  hexa						\
  histo_compute					\
  histo_to_image1d				\
  h_vec						\
						\
  image1d					\
  image2d					\
  image2d_h					\
  image3d					\
  image_if					\
  image_if_interval				\
  image_if_value				\
  interpolated					\
  io_pbm					\
  io_pgm16					\
  io_pgm19					\
  io_pgm27					\
  io_pgm					\
  io_ppm16					\
  io_ppm23					\
  io_ppm					\
						\
  l1						\
  l2						\
  linfty					\
  labeling_algo					\
  labeling_estimate				\
  labeling_foreground				\
  labeling_level_fast				\
  labeling_regional_maxima			\
  level_approx_median				\
  level_assign					\
  level_fill					\
  level_median					\
  level_median_dir				\
  level_median_fast				\
  level_median_hline2d				\
  level_memcpy_					\
  level_memset_					\
  level_naive_median				\
  level_paste					\
  level_sort_points				\
  level_transform				\
  line2d					\
  linear_convolve				\
  linear_gaussian				\
  linear_lap					\
  linear_log					\
  line_convolve					\
  line_piter					\
  literal_medium_gray				\
  literal_zero					\
  local_convolve				\
						\
  main						\
  mat						\
  mesh_image					\
  metal_converts_to				\
  metal_is					\
  metal_mat					\
  metal_pow					\
  metal_unconst					\
  metal_vec					\
  morpho_contrast				\
  morpho_dilation				\
  morpho_dilation_max_h				\
  morpho_erosion				\
  morpho_erosion_min_h				\
  morpho_gradient				\
  morpho_hit_or_miss				\
  morpho_laplacian				\
  morpho_opening_area				\
  morpho_thinning				\
						\
  new_io_pgm					\
  norm_l2					\
						\
  pixel						\
  pixter1d					\
  pixter2d					\
  pixter3d					\
  plain						\
  point1d					\
  point2d					\
  point3d					\
  pset_if					\
  pw_image					\
  pw_value					\
						\
  queue_p					\
  queue_p_fast					\
  queue_p_fast_priority				\
  queue_p_priority				\
						\
  rle_image					\
  run_pset					\
						\
  safe_image					\
  seed2tiling					\
  set_p						\
  show						\
  sparse_image					\
  stack						\
  sub_image					\
						\
  test_positive					\
  trait_ch_value				\
  trait_images					\
  trait_op_plus					\
  trait_op_uminus				\
  trait_super					\
  trait_value					\
  translate_image				\
  tr_image					\
						\
  value_float01_bis				\
  value_float01					\
  value_float01_f				\
  value_graylevel				\
  value_interop					\
  value_int_s16					\
  value_int_s					\
  value_int_u8					\
  value_label					\
  value_quat					\
  value_rgb8					\
  value_scalar					\
  value_set					\
  vec						\
						\
  win_backdiag2d				\
  win_diag2d					\
  win_disk2d					\
  window1d					\
  window2d					\
  window3d					\
  win_hline2d					\
  win_octagon2d					\
  win_rectangle2d				\
  w_window1d_int				\
  w_window2d_int				\
  w_window3d_int

accu_all_SOURCES = accu_all.cc
accu_min_SOURCES = accu_min.cc
accu_nil_SOURCES = accu_nil.cc

border_get_SOURCES = border_get.cc
border_resize_image1d_1_SOURCES = border_resize_image1d_1.cc
border_resize_image1d_2_SOURCES = border_resize_image1d_2.cc
border_resize_image1d_3_SOURCES = border_resize_image1d_3.cc
border_resize_image2d_1_SOURCES = border_resize_image2d_1.cc
border_resize_image2d_2_SOURCES = border_resize_image2d_2.cc
border_resize_image2d_3_SOURCES = border_resize_image2d_3.cc
border_resize_image3d_1_SOURCES = border_resize_image3d_1.cc
border_resize_image3d_2_SOURCES = border_resize_image3d_2.cc
border_resize_image3d_3_SOURCES = border_resize_image3d_3.cc
border_resize_image_if_SOURCES = border_resize_image_if.cc
border_resize_sub_image_SOURCES = border_resize_sub_image.cc
box1d_SOURCES = box1d.cc
box2d_SOURCES = box2d.cc
box3d_SOURCES = box3d.cc

canvas_browsing_fwd_SOURCES = canvas_browsing_fwd.cc
cast_image_SOURCES = cast_image.cc
chamfer_SOURCES = chamfer.cc
color_pretty_SOURCES = color_pretty.cc
convert_to_image_SOURCES = convert_to_image.cc
convert_to_tiles_SOURCES = convert_to_tiles.cc
convert_to_vec_p_SOURCES = convert_to_vec_p.cc
core_category_SOURCES = core_category.cc
core_clone_SOURCES = core_clone.cc
core_exact_SOURCES = core_exact.cc
core_initialize_SOURCES = core_initialize.cc

debug_println_SOURCES = debug_println.cc
debug_println_with_border_SOURCES = debug_println_with_border.cc
decorated_image_SOURCES = decorated_image.cc
dpoint1d_SOURCES = dpoint1d.cc
dpoint2d_SOURCES = dpoint2d.cc
dpoint3d_SOURCES = dpoint3d.cc
dpoints_pixter_SOURCES = dpoints_pixter.cc

estim_mean_SOURCES = estim_mean.cc

fun_x2x_composed_SOURCES = fun_x2x_composed.cc
fun_x2x_rotation_SOURCES = fun_x2x_rotation.cc
fun_x2x_translation_SOURCES = fun_x2x_translation.cc

hexa_SOURCES = hexa.cc
histo_compute_SOURCES = histo_compute.cc
histo_to_image1d_SOURCES = histo_to_image1d.cc
h_vec_SOURCES = h_vec.cc

image1d_SOURCES = image1d.cc
image2d_SOURCES = image2d.cc
image2d_h_SOURCES = image2d_h.cc
image3d_SOURCES = image3d.cc
image_if_SOURCES = image_if.cc
image_if_interval_SOURCES = image_if_interval.cc
image_if_value_SOURCES = image_if_value.cc
interpolated_SOURCES = interpolated.cc
io_pbm_SOURCES = io_pbm.cc
io_pgm16_SOURCES = io_pgm16.cc
io_pgm19_SOURCES = io_pgm19.cc
io_pgm27_SOURCES = io_pgm27.cc
io_pgm_SOURCES = io_pgm.cc
io_ppm16_SOURCES = io_ppm16.cc
io_ppm23_SOURCES = io_ppm23.cc
io_ppm_SOURCES = io_ppm.cc

l1_SOURCES = l1.cc
l2_SOURCES = l2.cc
linfty_SOURCES = linfty.cc
labeling_algo_SOURCES = labeling_algo.cc
labeling_estimate_SOURCES = labeling_estimate.cc
labeling_foreground_SOURCES = labeling_foreground.cc
labeling_level_fast_SOURCES = labeling_level_fast.cc
labeling_regional_maxima_SOURCES = labeling_regional_maxima.cc
level_approx_median_SOURCES = level_approx_median.cc
level_assign_SOURCES = level_assign.cc
level_fill_SOURCES = level_fill.cc
level_median_SOURCES = level_median.cc
level_median_dir_SOURCES = level_median_dir.cc
level_median_fast_SOURCES = level_median_fast.cc
level_median_hline2d_SOURCES = level_median_hline2d.cc
level_memcpy__SOURCES = level_memcpy_.cc
level_memset__SOURCES = level_memset_.cc
level_naive_median_SOURCES = level_naive_median.cc
level_paste_SOURCES = level_paste.cc
level_sort_points_SOURCES = level_sort_points.cc
level_transform_SOURCES = level_transform.cc
line2d_SOURCES = line2d.cc
linear_convolve_SOURCES = linear_convolve.cc
linear_gaussian_SOURCES = linear_gaussian.cc
linear_lap_SOURCES = linear_lap.cc
linear_log_SOURCES = linear_log.cc
line_convolve_SOURCES = line_convolve.cc
line_piter_SOURCES = line_piter.cc
literal_medium_gray_SOURCES = literal_medium_gray.cc
literal_zero_SOURCES = literal_zero.cc
local_convolve_SOURCES = local_convolve.cc

main_SOURCES = main.cc
mat_SOURCES = mat.cc
mesh_image_SOURCES = mesh_image.cc
metal_converts_to_SOURCES = metal_converts_to.cc
metal_is_SOURCES = metal_is.cc
metal_mat_SOURCES = metal_mat.cc
metal_pow_SOURCES = metal_pow.cc
metal_unconst_SOURCES = metal_unconst.cc
metal_vec_SOURCES = metal_vec.cc
morpho_contrast_SOURCES = morpho_contrast.cc
morpho_dilation_SOURCES = morpho_dilation.cc
morpho_dilation_max_h_SOURCES = morpho_dilation_max_h.cc
morpho_erosion_SOURCES = morpho_erosion.cc
morpho_erosion_min_h_SOURCES = morpho_erosion_min_h.cc
morpho_gradient_SOURCES = morpho_gradient.cc
morpho_hit_or_miss_SOURCES = morpho_hit_or_miss.cc
morpho_laplacian_SOURCES = morpho_laplacian.cc
morpho_opening_area_SOURCES = morpho_opening_area.cc
morpho_thinning_SOURCES = morpho_thinning.cc

new_io_pgm_SOURCES = new_io_pgm.cc
norm_l2_SOURCES = norm_l2.cc

pixel_SOURCES = pixel.cc
pixter1d_SOURCES = pixter1d.cc
pixter2d_SOURCES = pixter2d.cc
pixter3d_SOURCES = pixter3d.cc
plain_SOURCES = plain.cc
point1d_SOURCES = point1d.cc
point2d_SOURCES = point2d.cc
point3d_SOURCES = point3d.cc
pset_if_SOURCES = pset_if.cc
pw_image_SOURCES = pw_image.cc
pw_value_SOURCES = pw_value.cc

queue_p_SOURCES = queue_p.cc
queue_p_fast_SOURCES = queue_p_fast.cc
queue_p_fast_priority_SOURCES = queue_p_fast_priority.cc
queue_p_priority_SOURCES = queue_p_priority.cc

rle_image_SOURCES = rle_image.cc
run_pset_SOURCES = run_pset.cc

safe_image_SOURCES = safe_image.cc
seed2tiling_SOURCES = seed2tiling.cc
set_p_SOURCES = set_p.cc
show_SOURCES = show.cc
sparse_image_SOURCES = sparse_image.cc
stack_SOURCES = stack.cc
sub_image_SOURCES = sub_image.cc

test_positive_SOURCES = test_positive.cc
trait_ch_value_SOURCES = trait_ch_value.cc
trait_images_SOURCES = trait_images.cc
trait_op_plus_SOURCES = trait_op_plus.cc
trait_op_uminus_SOURCES = trait_op_uminus.cc
trait_super_SOURCES = trait_super.cc
trait_value_SOURCES = trait_value.cc
translate_image_SOURCES = translate_image.cc
tr_image_SOURCES = tr_image.cc

value_float01_bis_SOURCES = value_float01_bis.cc
value_float01_SOURCES = value_float01.cc
value_float01_f_SOURCES = value_float01_f.cc
value_graylevel_SOURCES = value_graylevel.cc
value_interop_SOURCES = value_interop.cc
value_int_s16_SOURCES = value_int_s16.cc
value_int_s_SOURCES = value_int_s.cc
value_int_u8_SOURCES = value_int_u8.cc
value_label_SOURCES = value_label.cc
value_quat_SOURCES = value_quat.cc
value_rgb8_SOURCES = value_rgb8.cc
value_scalar_SOURCES = value_scalar.cc
value_set_SOURCES = value_set.cc
vec_SOURCES = vec.cc

win_backdiag2d_SOURCES = win_backdiag2d.cc
win_diag2d_SOURCES = win_diag2d.cc
win_disk2d_SOURCES = win_disk2d.cc
window1d_SOURCES = window1d.cc
window2d_SOURCES = window2d.cc
window3d_SOURCES = window3d.cc
win_hline2d_SOURCES = win_hline2d.cc
win_octagon2d_SOURCES = win_octagon2d.cc
win_rectangle2d_SOURCES = win_rectangle2d.cc
w_window1d_int_SOURCES = w_window1d_int.cc
w_window2d_int_SOURCES = w_window2d_int.cc
w_window3d_int_SOURCES = w_window3d_int.cc

# Tests depending on the FreeImage library.
if FREEIMAGEPLUS
  check_PROGRAMS += fi_adaptor
  fi_adaptor_SOURCES = fi_adaptor.cc
  fi_adaptor_CPPFLAGS = $(AM_CPPFLAGS) $(FREEIMAGEPLUS_CPPFLAGS)
  fi_adaptor_LDFLAGS = $(AM_LDFLAGS) $(FREEIMAGEPLUS_LDFLAGS)
endif

TESTS = $(check_PROGRAMS)
