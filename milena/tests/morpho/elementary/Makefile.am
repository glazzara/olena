# Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE).
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
  laplacian 					\
  dilation 					\
  gradient_internal 				\
  opening 					\
  closing 					\
  top_hat 					\
  gradient_external 				\
  gradient 					\
  erosion

laplacian_SOURCES = laplacian.cc
dilation_SOURCES = dilation.cc
gradient_internal_SOURCES = gradient_internal.cc
opening_SOURCES = opening.cc
closing_SOURCES = closing.cc
top_hat_SOURCES = top_hat.cc
gradient_external_SOURCES = gradient_external.cc
gradient_SOURCES = gradient.cc
erosion_SOURCES = erosion.cc

TESTS = $(check_PROGRAMS)
