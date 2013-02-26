## tests-recursive.mk				-*- Makefile-Automake -*-

## Compile tests recursively, but don't run them.

## Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
## (LRDE)
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/build-aux/extra-recursive-targets.mk

# Sometimes, we just want to check whether tests compile, without
# actually running them.  Hence this `tests' target, which does half
# the job of the `check' target.
EXTRA_RECURSIVE_TARGETS += tests-recursive
.PHONY: tests-am tests-recursive tests
# ``Halved'' version of check-am.
tests-am: $(check_PROGRAMS)
	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
tests: tests-recursive
