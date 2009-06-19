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
#

## Process this file through Automake to create Makefile.in.

include $(top_srcdir)/milena/tests/tests.mk

# Regen files.
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	cd $(top_srcdir)					\
	&& ./build-aux/build_unit_test.sh 			\
		milena/mln 					\
		milena/tests/unit_test 				\
		milena/tests/unit_test/disabled_tests

# FIXME: Change build_unit_test.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
# `$(abs_top_srcdir)' is required because of the change of directory
# at the beginning og the action.
$(srcdir)/unit-tests.mk: $(top_srcdir)/build-aux/build_unit_test.sh
	cd $(top_srcdir)					\
	&& ./build-aux/build_unit_test.sh 			\
		milena/mln 					\
		milena/tests/unit_test 				\
		milena/tests/unit_test/disabled_tests

include $(srcdir)/unit-tests.mk

MAINTAINERCLEANFILES = $(srcdir)/mln_*.cc

TESTS = $(check_PROGRAMS)
