# Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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
include $(top_srcdir)/scribo/scribo.mk

utilexec_PROGRAMS = scribo-xml2doc
scribo_xml2doc_SOURCES = common.cc 		\
			 image_crop.cc 		\
			 loader.cc		\
			 main.cc 		\
			 xml_transform.cc

nodist_scribo_xml2doc_SOURCES = $(BUILT_SOURCES)

scribo_xml2doc_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) \
			  $(MAGICKXX_CPPFLAGS) 		\
			  -I$(top_srcdir)/milena 	\
			  -I$(top_srcdir)/scribo 	\
			  -DNDEBUG -DMLN_WO_GLOBAL_VARS
scribo_xml2doc_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
scribo_xml2doc_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS) $(MAGICKXX_LDFLAGS)
scribo_xml2doc_LDADD    = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = common.moc.cc			\
		image_crop.moc.cc		\
		loader.moc.cc			\
		xml_transform.moc.cc


noinst_HEADERS = common.hh 			\
		 image_crop.hh 			\
		 loader.hh 			\
		 xml_transform.hh 		\
		 datarootdir.hh

# Generated files
noinst_gen_files = datarootdir.hh
noinst_HEADERS += $(noinst_gen_files)

do_subst = sed  					\
	   -e 's|@datarootdir[@]|$(datarootdir)|g'	\
	   -e 's|@abs_srcdir[@]|$(abs_srcdir)|g'

datarootdir.hh: $(srcdir)/datarootdir.hh.in Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(do_subst) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES = $(noinst_gen_files) $(BUILT_SOURCES)

EXTRA_DIST = 			\
	README 			\
	datarootdir.hh.in

