## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  dual_neighb					\
  dummy_p_edges 				\
  dummy_p_vertices 				\
  influence_zone_adjacency_graph		\
  h_mat 					\
  image2d					\
  image3d					\
  mat						\
  p_edges_with_mass_centers 			\
  p_vertices_with_mass_centers 			\
  region_adjacency_graph 			\
  rag_and_labeled_wsl				\
  w_window					\
  w_window_directional

dual_neighb_SOURCES = dual_neighb.c
dual_neighb_SOURCES = dual_neighb.cc
dummy_p_edges_SOURCES = dummy_p_edges.cc
dummy_p_vertices_SOURCES = dummy_p_vertices.cc
influence_zone_adjacency_graph_SOURCES = influence_zone_adjacency_graph.cc
h_mat_SOURCES = h_mat.cc
image2d_SOURCES = image2d.cc
image3d_SOURCES = image3d.cc
mat_SOURCES = mat.cc
p_edges_with_mass_centers_SOURCES = p_edges_with_mass_centers.cc
p_vertices_with_mass_centers_SOURCES = p_vertices_with_mass_centers.cc
region_adjacency_graph_SOURCES = region_adjacency_graph.cc
rag_and_labeled_wsl_SOURCES = rag_and_labeled_wsl.cc
w_window_SOURCES = w_window.cc
w_window_directional_SOURCES = w_window_directional.cc

TESTS = $(check_PROGRAMS)
