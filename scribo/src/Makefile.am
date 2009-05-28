## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/scribo/scribo.mk

recognition_CXXFLAGS = -I$(HOME)/local/include $(HOME)/local/lib/libtesseract_full.a
recognition_LDFLAGS = -lpthread

SUBDIRS =					\
  binarization 					\
  preprocessing 				\
  table

bin_PROGRAMS = 				\
  dmap                               	\
  extract_text_double_link           	\
  extract_text_double_several_links  	\
  extract_text_graph                 	\
  extract_text_several_graph         	\
  extract_text_several_left_links    	\
  extract_text_single_link  		\
  morpho                    		\
  negate 				\
  recognition 				\
  superpose 				\
  table_rebuild_opening 		\
  table_rebuild_rank 			\
  thin_bboxes

dmap_SOURCES = dmap.cc                               
extract_text_double_link_SOURCES = extract_text_double_link.cc           
extract_text_double_several_links_SOURCES = extract_text_double_several_links.cc  
extract_text_graph_SOURCES = extract_text_graph.cc                 
extract_text_several_graph_SOURCES = extract_text_several_graph.cc         
extract_text_several_left_links_SOURCES = extract_text_several_left_links.cc    
extract_text_single_link_SOURCES = extract_text_single_link.cc  
morpho_SOURCES = morpho.cc               
negate_SOURCES = negate.cc
recognition_SOURCES = recognition.cc
superpose_SOURCES = superpose.cc
table_rebuild_opening_SOURCES = table_rebuild_opening.cc
table_rebuild_rank_SOURCES = table_rebuild_rank.cc
thin_bboxes_SOURCES = thin_bboxes.cc

