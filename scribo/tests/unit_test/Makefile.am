# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).
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

include $(top_srcdir)/scribo/tests/tests.mk

EXTRA_DIST = build_unit_test.sh disabled_tests

# Regen files.
include $(top_srcdir)/build-aux/regen-recursive.mk
regen-am:
	cd $(top_srcdir)					\
	&& scribo/tests/unit_test/build_unit_test.sh 		\
		scribo	 					\
		scribo/tests/unit_test 				\
		scribo/tests/unit_test/disabled_tests

# FIXME: Change build_unit_test.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
# `$(abs_top_srcdir)' is required because of the change of directory
# at the beginning og the action.
$(srcdir)/unit-tests.mk: build_unit_test.sh disabled_tests
	cd $(top_srcdir)					\
	&& scribo/tests/unit_test/build_unit_test.sh		\
		scribo	 					\
		scribo/tests/unit_test 				\
		scribo/tests/unit_test/disabled_tests

include $(srcdir)/unit-tests.mk


#FIXME: how to handle that automaticaly during the unit test list
#generation?
scribo_convert_from_qimage_CPPFLAGS=\
	$(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir)
scribo_convert_from_qimage_CXXFLAGS=\
	$(QT_CXXFLAGS) $(AM_CXXFLAGS)
scribo_convert_from_qimage_LDFLAGS=\
	$(QT_LDFLAGS) $(LDFLAGS)
scribo_convert_from_qimage_LDADD=\
	$(QT_LIBS) $(LDADD)

scribo_toolchain_nepomuk_text_extraction_CPPFLAGS=\
	$(QT_CPPFLAGS) $(TESSERACT_CPPFLAGS) $(TIFF_CPPFLAGS) \
	$(AM_CPPFLAGS) -I$(srcdir)
scribo_toolchain_nepomuk_text_extraction_CXXFLAGS=\
	$(QT_CXXFLAGS) $(AM_CXXFLAGS)
scribo_toolchain_nepomuk_text_extraction_LDFLAGS=\
	$(QT_LDFLAGS) $(TESSERACT_LDFLAGS) $(TIFF_LDFLAGS) \
	$(LDFLAGS)
scribo_toolchain_nepomuk_text_extraction_LDADD=\
	$(QT_LIBS) $(LDADD)

scribo_text_recognition_CPPFLAGS=\
	$(TESSERACT_CPPFLAGS) $(TIFF_CPPFLAGS) \
	$(AM_CPPFLAGS) -I$(srcdir)
scribo_text_recognition_LDFLAGS=\
	$(TESSERACT_LDFLAGS) $(TIFF_LDFLAGS) \
	$(LDFLAGS)



MAINTAINERCLEANFILES = $(srcdir)/scribo_*.cc

TESTS = $(check_PROGRAMS)
