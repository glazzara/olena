# Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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
  background					\
  blobs						\
  blobs_and_compute				\
  compute 					\
  fill_holes					\
  flat_zones					\
  foreground					\
  mean_values 					\
  n_max						\
  pack 						\
  regional_maxima				\
  regional_minima 				\
  relabel 					\
  value						\
  wrap

### FIXME: Disable this test, since labeling::colorize depends on
### rand(), which does not behave uniformly among the various targeted
### platforms.
## check_PROGRAMS +=				\
##   colorize

TESTS = $(check_PROGRAMS)
