## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

##FIXME: re-enable tests
check_PROGRAMS =				\
  box_runstart_piter				\
  category					\
##  clock_neighb2d				\
##  clock_test					\
  dpoints_pixter				\
  graph_elt_neighborhood			\
  graph_elt_window				\
  neighb					\
  pixel						\
  pixter1d					\
  pixter1d_more					\
  pixter2d					\
  pixter2d_more					\
  pixter3d					\
  pixter3d_more					\
  point_set_compatibility			\
  var 						\
  w_window

box_runstart_piter_SOURCES = box_runstart_piter.cc
category_SOURCES = category.cc
##clock_neighb2d_SOURCES = clock_neighb2d.cc
##clock_test_SOURCES = clock_test.cc
dpoints_pixter_SOURCES = dpoints_pixter.cc
graph_elt_neighborhood_SOURCES = graph_elt_neighborhood.cc
graph_elt_window_SOURCES = graph_elt_window.cc
neighb_SOURCES = neighb.cc
pixel_SOURCES = pixel.cc
pixter1d_SOURCES = pixter1d.cc
pixter1d_more_SOURCES = pixter1d_more.cc
pixter2d_SOURCES = pixter2d.cc
pixter2d_more_SOURCES = pixter2d_more.cc
pixter3d_SOURCES = pixter3d.cc
pixter3d_more_SOURCES = pixter3d_more.cc
point_set_compatibility_SOURCES = point_set_compatibility.cc
var_SOURCES = var.cc
w_window_SOURCES = w_window.cc

TESTS = $(check_PROGRAMS)
