# Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

include $(top_srcdir)/scribo/scribo.mk
include $(top_srcdir)/build-aux/autotroll.mk


bin_PROGRAMS = scribo-viewer
scribo_viewer_SOURCES = viewer.cc		\
			main.cc			\
			image_widget.cc		\
			xml_widget.cc		\
			key_widget.cc		\
			browser_widget.cc	\
			image_scene.cc		\
			image_view.cc		\
			image_region.cc		\
			help_dialog.cc		\
			step_widget.cc 		\
			preprocessing_options.cc\
			segmentation_options.cc \
			ocr_options.cc 		\
			runner.cc		\
			preprocess.cc		\
			process.cc		\
			general_options.cc 	\
			preferences_dialog.cc 	\
			option_widget.cc 	\
			config.cc

nodist_scribo_viewer_SOURCES = $(BUILT_SOURCES)

scribo_viewer_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir) 	\
			 -I$(top_srcdir)/milena				\
			 -I$(top_srcdir)/scribo 			\
			 $(MAGICKXX_CPPFLAGS) 				\
			 $(TESSERACT_CPPFLAGS) 				\
		         $(TIFF_CPPFLAGS) 				\
			 -DNDEBUG -DMLN_WO_GLOBAL_VARS
scribo_viewer_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) 			\
			 $(STRICT_ALIASING_CXXFLAGS) -O3
scribo_viewer_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS) 			\
			 $(MAGICKXX_LDFLAGS)				\
			 $(TESSERACT_LDFLAGS)				\
		         $(TIFF_LDFLAGS)
scribo_viewer_LDADD    = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = viewer.moc.cc			\
		image_widget.moc.cc		\
		xml_widget.moc.cc		\
		key_widget.moc.cc		\
		browser_widget.moc.cc		\
		image_scene.moc.cc		\
		image_view.moc.cc		\
		image_region.moc.cc		\
		help_dialog.ui.h		\
		step_widget.moc.cc 		\
		preprocessing_options.moc.cc	\
		preprocessing_options.ui.h	\
		segmentation_options.moc.cc	\
		segmentation_options.ui.h	\
		ocr_options.moc.cc		\
		ocr_options.ui.h		\
		runner.moc.cc 			\
		preprocess.moc.cc		\
		process.moc.cc			\
		preferences_dialog.moc.cc	\
		preferences_dialog.ui.h		\
		general_options.moc.cc		\
		general_options.ui.h		\
		demodir.hh 			\
		viewer.qrc.cc


noinst_HEADERS = viewer.hh		\
	 	 viewer.hxx		\
	 	 image_widget.hh	\
	 	 xml_widget.hh		\
	 	 key_widget.hh		\
	 	 browser_widget.hh	\
	 	 image_scene.hh		\
	 	 image_view.hh		\
	 	 image_region.hh	\
	 	 image_region.hxx	\
	 	 common.hh		\
	 	 help_dialog.hh		\
	 	 step_widget.hh		\
		 preprocessing_options.hh\
		 segmentation_options.hh\
		 ocr_options.hh		\
		 runner.hh 		\
		 preprocess.hh		\
		 process.hh 		\
		 general_options.hh 	\
		 preferences_dialog.hh 	\
		 option_widget.hh 	\
		 config.hh	 	\
		 defs.hh


# Generated files
do_subst = sed  					\
	   -e 's|@libexecdir[@]|$(libexecdir)|g'		\
	   -e 's|@abs_demodir[@]|$(abs_top_builddir)/scribo/demo|g'

demodir.hh: $(srcdir)/demodir.hh.in Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	test -f ./$@.in || srcdir=$(srcdir)/; \
	$(do_subst) $${srcdir}$@.in >$@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST =			\
	general_options.ui	\
	preferences_dialog.ui 	\
	README			\
	preprocessing_options.ui\
	segmentation_options.ui	\
	ocr_options.ui		\
	help_dialog.ui 		\
	demodir.hh.in		\
	viewer.qrc		\
	icons/olena_smaller.jpg

