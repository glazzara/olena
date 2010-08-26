##
## Document layout viewer.
##
## Copyright (C) 2009 Florent D'Halluin.
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

include $(top_srcdir)/build-aux/autotroll.mk

bin_PROGRAMS = scribo-viewer
scribo_viewer_SOURCES = $(BUILT_SOURCES)	\
			viewer.cc		\
			main.cc			\
			image_widget.cc		\
			xml_widget.cc		\
			key_widget.cc		\
			property_widget.cc	\
			browser_widget.cc	\
			image_scene.cc		\
			image_view.cc		\
			dommodel.cc		\
			domitem.cc		\
			image_region.cc		\
			help_dialog.cc		\
			step_widget.cc

scribo_viewer_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS) -I$(srcdir)
scribo_viewer_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -O3
scribo_viewer_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)
scribo_viewer_LDADD    = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = viewer.moc.cc			\
		image_widget.moc.cc		\
		xml_widget.moc.cc		\
		key_widget.moc.cc		\
		property_widget.moc.cc		\
		browser_widget.moc.cc		\
		image_scene.moc.cc		\
		image_view.moc.cc		\
		dommodel.moc.cc			\
		domitem.moc.cc			\
		image_region.moc.cc		\
		help_dialog.moc.cc		\
		step_widget.moc.cc

noinst_HEADERS = viewer.hh		\
	 	 viewer.hxx		\
	 	 image_widget.hh	\
	 	 xml_widget.hh		\
	 	 key_widget.hh		\
	 	 property_widget.hh	\
	 	 browser_widget.hh	\
	 	 image_scene.hh		\
	 	 image_view.hh		\
	 	 dommodel.hh		\
	 	 domitem.hh		\
	 	 image_region.hh	\
	 	 image_region.hxx	\
	 	 common.hh		\
	 	 help_dialog.hh		\
	 	 step_widget.hh

EXTRA_DIST = README
