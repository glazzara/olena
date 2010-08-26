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

include $(top_srcdir)/build-aux/autotroll.mk

bin_PROGRAMS = scribo-review
scribo_review_SOURCES = $(BUILT_SOURCES)	\
			src/main.cc		\
		 	src/mainwindow.cc

scribo_review_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)	\
		  	 -DNDEBUG 			\
		  	 -I$(srcdir) 			\
			 -I$(builddir)/ui
scribo_review_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
scribo_review_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
scribo_review_LDADD    = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = src/mainwindow.moc.cc \
		ui/mainwindow.ui.h

noinst_HEADERS = src/mainwindow.hh



EXTRA_DIST = 	\
  README	\
  review.qrc
