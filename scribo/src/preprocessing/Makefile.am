# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
# (LRDE).
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
  denoise_bg 				\
  denoise_fg 				\
  homogeneous_contrast 			\
  preprocess 				\
  unskew

denoise_bg_SOURCES = denoise_bg.cc
denoise_fg_SOURCES = denoise_fg.cc
homogeneous_contrast_SOURCES = homogeneous_contrast.cc
preprocess_SOURCES = preprocess.cc
unskew_SOURCES = unskew.cc

if HAVE_MAGICKXX

    noinst_PROGRAMS    	+= split_bg_fg
    split_bg_fg_SOURCES  = split_bg_fg.cc
    split_bg_fg_CPPFLAGS = $(AM_CPPFLAGS) \
		           $(MAGICKXX_CPPFLAGS)
    split_bg_fg_LDFLAGS  = $(AM_LDFLAGS) \
			   $(MAGICKXX_LDFLAGS) \
			   -lpthread

    noinst_PROGRAMS += rotate
    rotate_SOURCES  = rotate.cc
    rotate_CPPFLAGS = $(AM_CPPFLAGS) \
	              $(MAGICKXX_CPPFLAGS)
    rotate_LDFLAGS  = $(AM_LDFLAGS) \
		      $(MAGICKXX_LDFLAGS) \
		      -lpthread

    noinst_PROGRAMS   += rotate_90
    rotate_90_SOURCES  = rotate_90.cc
    rotate_90_CPPFLAGS = $(AM_CPPFLAGS) \
	                 $(MAGICKXX_CPPFLAGS)
    rotate_90_LDFLAGS  = $(AM_LDFLAGS) \
		         $(MAGICKXX_LDFLAGS) \
			 -lpthread

    noinst_PROGRAMS += to_pgm
    to_pgm_SOURCES   = to_pgm.cc
    to_pgm_CPPFLAGS  = $(AM_CPPFLAGS) \
		       $(MAGICKXX_CPPFLAGS)
    to_pgm_LDFLAGS   = $(AM_LDFLAGS) \
		       $(MAGICKXX_LDFLAGS) \
		       -lpthread

    noinst_PROGRAMS   += subsample
    subsample_SOURCES  = subsample.cc
    subsample_CPPFLAGS = $(AM_CPPFLAGS) \
			 $(MAGICKXX_CPPFLAGS)
    subsample_LDFLAGS  = $(AM_LDFLAGS) \
		         $(MAGICKXX_LDFLAGS) \
		         -lpthread

endif HAVE_MAGICKXX
