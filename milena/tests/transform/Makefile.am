# Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  bench_closest_point_geodesic			\
  distance_and_closest_point_geodesic		\
  distance_front				\
  distance_geodesic				\
  hough 					\
  influence_zone_front				\
  influence_zone_geodesic 			\
  influence_zone_geodesic_saturated



if HAVE_FFTW3

check_PROGRAMS += fft
fft_SOURCES = fft.cc
fft_CPPFLAGS = $(AM_CPPFLAGS) $(FFTW3_CPPFLAGS)
fft_LDFLAGS = $(AM_LDFLAGS) $(FFTW3_LDFLAGS)

endif HAVE_FFTW3


TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = 				\
  fft.pgm					\
  fft_copy.pgm					\
  fft_low_pass.pgm				\
  fft_trans_clipped.pgm				\
  fft_trans_cropped.pgm				\
  fft_trans_log.pgm
