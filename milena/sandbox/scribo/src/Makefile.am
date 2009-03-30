## Process this file through Automake to create Makefile.in -*- Makefile -*-

include $(top_srcdir)/milena/sandbox/scribo/scribo.mk

SUBDIRS =					\
  binarization

bin_PROGRAMS = 				\
  dmap                               	\
  extract_text_single_link  		\
  table_erase 				\
  extract_text_double_link           	\
  table_extract 			\
  extract_text_double_several_links  	\
  morpho                    		\
  extract_text_graph                 	\
  table_rebuild_opening 		\
  extract_text_several_graph         	\
  table_rebuild_rank 			\
  extract_text_several_left_links    	\
  thin_bboxes

dmap_SOURCES = dmap.cc                               
extract_text_single_link_SOURCES = extract_text_single_link.cc  
table_erase_SOURCES = table_erase.cc
extract_text_double_link_SOURCES = extract_text_double_link.cc           
table_extract_SOURCES = table_extract.cc
extract_text_double_several_links_SOURCES = extract_text_double_several_links.cc  
morpho_SOURCES = morpho.cc                    
extract_text_graph_SOURCES = extract_text_graph.cc                 
table_rebuild_opening_SOURCES = table_rebuild_opening.cc
extract_text_several_graph_SOURCES = extract_text_several_graph.cc         
table_rebuild_rank_SOURCES = table_rebuild_rank.cc
extract_text_several_left_links_SOURCES = extract_text_several_left_links.cc    
thin_bboxes_SOURCES = thin_bboxes.cc

