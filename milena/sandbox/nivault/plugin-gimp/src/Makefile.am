## Process this file with automake to produce Makefile.in

bin_PROGRAMS = milena

bindir = $(GIMP_LIBDIR)/plug-ins

milena_SOURCES = \
	build-image.hh	\
	build-image.hxx	\
	main.cc		\
	plugin-intl.hh	\
	interface.hxx	\
	interface.hh

AM_CPPFLAGS = \
	-DLOCALEDIR=\""$(LOCALEDIR)"\"		\
	-DDATADIR=\""$(DATADIR)"\"

INCLUDES =\
	-I$(top_srcdir)				\
	-I../../../../				\
	@GIMP_CFLAGS@				\
	-I$(includedir)

LDADD = $(GIMP_LIBS)

