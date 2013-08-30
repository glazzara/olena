# Copyright (C) 2009, 2010, 2013 EPITA Research and Development
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


if HAVE_MAGICKXX

  noinst_PROGRAMS += global_threshold sauvola_debug sauvola_ms_debug

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


  global_threshold_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  	      $(MAGICKXX_CPPFLAGS)
  global_threshold_LDFLAGS  = $(AM_LDFLAGS) 		\
		              $(MAGICKXX_LDFLAGS)


  niblack_CPPFLAGS = $(AM_CPPFLAGS) 		\
		     $(MAGICKXX_CPPFLAGS)
  niblack_LDFLAGS  = $(AM_LDFLAGS) 		\
		     $(MAGICKXX_LDFLAGS)

  otsu_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  $(MAGICKXX_CPPFLAGS)
  otsu_LDFLAGS  = $(AM_LDFLAGS) 		\
		  $(MAGICKXX_LDFLAGS)

  sauvola_ms_fg_CPPFLAGS = $(AM_CPPFLAGS) 	\
			   $(MAGICKXX_CPPFLAGS)
  sauvola_ms_fg_LDFLAGS  = $(AM_LDFLAGS) 	\
			   $(MAGICKXX_LDFLAGS)

  kim_CPPFLAGS = $(AM_CPPFLAGS) 	\
		 $(MAGICKXX_CPPFLAGS)
  kim_LDFLAGS  = $(AM_LDFLAGS) 	\
		 $(MAGICKXX_LDFLAGS)

  sauvola_CPPFLAGS = $(AM_CPPFLAGS) 		\
		     $(MAGICKXX_CPPFLAGS)
  sauvola_LDFLAGS  = $(AM_LDFLAGS) 		\
		     $(MAGICKXX_LDFLAGS)

  singh_CPPFLAGS = $(AM_CPPFLAGS) 		\
		   $(MAGICKXX_CPPFLAGS)
  singh_LDFLAGS  = $(AM_LDFLAGS) 		\
		   $(MAGICKXX_LDFLAGS)

  wolf_CPPFLAGS = $(AM_CPPFLAGS) 		\
		  $(MAGICKXX_CPPFLAGS)
  wolf_LDFLAGS  = $(AM_LDFLAGS) 		\
		  $(MAGICKXX_LDFLAGS)

  sauvola_debug_CPPFLAGS = $(AM_CPPFLAGS) 	  \
			   -DSCRIBO_LOCAL_THRESHOLD_DEBUG \
			   $(MAGICKXX_CPPFLAGS)
  sauvola_debug_LDFLAGS  = $(AM_LDFLAGS) 	\
			   $(MAGICKXX_LDFLAGS)


  sauvola_ms_CPPFLAGS = $(AM_CPPFLAGS) 		\
			$(MAGICKXX_CPPFLAGS)
  sauvola_ms_LDFLAGS  = $(AM_LDFLAGS) 		\
		        $(MAGICKXX_LDFLAGS)

  sauvola_ms_debug_CPPFLAGS = $(AM_CPPFLAGS) 		\
			      -DSCRIBO_LOCAL_THRESHOLD_DEBUG \
			      $(MAGICKXX_CPPFLAGS)
  sauvola_ms_debug_LDFLAGS  = $(AM_LDFLAGS) 		\
		              $(MAGICKXX_LDFLAGS)


  sauvola_ms_split_CPPFLAGS = $(AM_CPPFLAGS) 		\
			      $(MAGICKXX_CPPFLAGS)
  sauvola_ms_split_LDFLAGS  = $(AM_LDFLAGS) 		\
			      $(MAGICKXX_LDFLAGS)


endif HAVE_MAGICKXX
