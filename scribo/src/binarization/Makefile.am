# Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
# Laboratory (LRDE).
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

include $(top_srcdir)/scribo/scribo.mk

noinst_PROGRAMS = 			\
  pgm_global_threshold_auto

pgm_global_threshold_auto_SOURCES = pgm_global_threshold_auto.cc

if HAVE_MAGICKXX

  noinst_PROGRAMS +=  			\
   global_threshold			\
   sauvola_debug 			\
   sauvola_ms_debug

  utilexec_PROGRAMS =                   \
   kim					\
   niblack				\
   otsu					\
   sauvola 				\
   sauvola_ms                           \
   sauvola_ms_fg 			\
   sauvola_ms_split			\
   singh				\
   wolf


  global_threshold_SOURCES  = global_threshold.cc
  global_threshold_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  	      $(MAGICKXX_CPPFLAGS)
  global_threshold_LDFLAGS  = $(AM_LDFLAGS) 		\
		              $(MAGICKXX_LDFLAGS)


  niblack_SOURCES  = niblack.cc
  niblack_CPPFLAGS = $(AM_CPPFLAGS) 		\
		     $(MAGICKXX_CPPFLAGS)
  niblack_LDFLAGS  = $(AM_LDFLAGS) 		\
		     $(MAGICKXX_LDFLAGS)


  otsu_SOURCES  = otsu.cc
  otsu_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  $(MAGICKXX_CPPFLAGS)
  otsu_LDFLAGS  = $(AM_LDFLAGS) 		\
		  $(MAGICKXX_LDFLAGS)

  sauvola_ms_fg_SOURCES  = sauvola_ms_fg.cc
  sauvola_ms_fg_CPPFLAGS = $(AM_CPPFLAGS) 	\
			   $(MAGICKXX_CPPFLAGS)
  sauvola_ms_fg_LDFLAGS  = $(AM_LDFLAGS) 	\
			   $(MAGICKXX_LDFLAGS)

  kim_SOURCES  = kim.cc
  kim_CPPFLAGS = $(AM_CPPFLAGS) 	\
		 $(MAGICKXX_CPPFLAGS)
  kim_LDFLAGS  = $(AM_LDFLAGS) 	\
		 $(MAGICKXX_LDFLAGS)

  sauvola_SOURCES  = sauvola.cc
  sauvola_CPPFLAGS = $(AM_CPPFLAGS) 		\
		     $(MAGICKXX_CPPFLAGS)
  sauvola_LDFLAGS  = $(AM_LDFLAGS) 		\
		     $(MAGICKXX_LDFLAGS)

  singh_SOURCES  = singh.cc
  singh_CPPFLAGS = $(AM_CPPFLAGS) 		\
		   $(MAGICKXX_CPPFLAGS)
  singh_LDFLAGS  = $(AM_LDFLAGS) 		\
		   $(MAGICKXX_LDFLAGS)

  wolf_SOURCES  = wolf.cc
  wolf_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  $(MAGICKXX_CPPFLAGS)
  wolf_LDFLAGS  = $(AM_LDFLAGS) 		\
		  $(MAGICKXX_LDFLAGS)

  sauvola_debug_SOURCES  = sauvola_debug.cc
  sauvola_debug_CPPFLAGS = $(AM_CPPFLAGS) 	  \
			   -DSCRIBO_LOCAL_THRESHOLD_DEBUG \
			   $(MAGICKXX_CPPFLAGS)
  sauvola_debug_LDFLAGS  = $(AM_LDFLAGS) 	\
			   $(MAGICKXX_LDFLAGS)


  sauvola_ms_SOURCES  = sauvola_ms.cc
  sauvola_ms_CPPFLAGS = $(AM_CPPFLAGS) 		\
			$(MAGICKXX_CPPFLAGS)
  sauvola_ms_LDFLAGS  = $(AM_LDFLAGS) 		\
		        $(MAGICKXX_LDFLAGS)

  sauvola_ms_debug_SOURCES  = sauvola_ms_debug.cc
  sauvola_ms_debug_CPPFLAGS = $(AM_CPPFLAGS) 		\
			      -DSCRIBO_LOCAL_THRESHOLD_DEBUG \
			      $(MAGICKXX_CPPFLAGS)
  sauvola_ms_debug_LDFLAGS  = $(AM_LDFLAGS) 		\
		              $(MAGICKXX_LDFLAGS)


  sauvola_ms_split_SOURCES  = sauvola_ms_split.cc
  sauvola_ms_split_CPPFLAGS = $(AM_CPPFLAGS) 		\
			      $(MAGICKXX_CPPFLAGS)
  sauvola_ms_split_LDFLAGS  = $(AM_LDFLAGS) 		\
			      $(MAGICKXX_LDFLAGS)


endif HAVE_MAGICKXX

