# Copyright (C) 2008, 2009, 2010, 2013, 2014 EPITA Research and Development
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

COND_TESTS = cond_tests_boost-tuple		\
	     cond_tests_cfitsio			\
	     cond_tests_fftw3			\
	     cond_tests_gdcm			\
	     cond_tests_magickxx		\
	     cond_tests_poppler			\
	     cond_tests_qt			\
	     cond_tests_tiff

EXTRA_DIST = disabled_tests \
	     $(COND_TESTS)

# Regen files.
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	cd $(top_srcdir)					\
	&& ./build-aux/build_unit_test.sh 			\
		milena/mln 					\
		milena/tests/unit_test 				\
		milena/tests/unit_test/disabled_tests 		\
		mln

# FIXME: Change build_unit_test.sh so that the action looks like this:
#
#	$(top_srcdir)/build-aux/build_unit_test.sh \
#         $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
# `$(abs_top_srcdir)' is required because of the change of directory
# at the beginning og the action.
$(srcdir)/unit-tests.mk: $(top_srcdir)/build-aux/build_unit_test.sh disabled_tests $(COND_TESTS)
	cd $(top_srcdir)					\
	&& ./build-aux/build_unit_test.sh 			\
		milena/mln 					\
		milena/tests/unit_test 				\
		milena/tests/unit_test/disabled_tests 		\
		mln

# Handling Boost.Tuple-dependent programs: `boost.m4' defines
# BOOST_CPPFLAGS, but `unit-tests.mk' expects BOOST_TUPLE_CPPFLAGS;
# help it by defining the later using the former.
BOOST_TUPLE_CPPFLAGS = $(BOOST_CPPFLAGS)

include $(srcdir)/unit-tests.mk

MAINTAINERCLEANFILES = $(srcdir)/mln_*.cc

TESTS = $(check_PROGRAMS)
