# Do not install me
lib_LTLIBRARIES = libdyn_function.la
libdyn_function_la_LDFLAGS = `dyn-config --libtool-libs`
libdyn_function_la_CXXFLAGS = `dyn-config --cflags`
libdyn_function_la_SOURCES = function.cc
BUILT_SOURCES = function.cc
CLEANFILES = function.cc
SUBDIRS =

pkgdata_DATA = Makefile.template Makefile.repository

Makefile.template: Makefile
	cat $< | \
	sed -e 's|^srcdir = .*|srcdir = '`cd $(srcdir) && pwd`'|' \
	    -e 's|^top_srcdir = .*|top_srcdir = '`cd $(top_srcdir) && pwd`'|' \
	    -e 's|^top_builddir = .*|top_builddir = '`cd $(top_builddir) && pwd`'|' \
	    -e 's|^pkgdata_DATA = .*|pkgdata_DATA = |' \
	    -e 's|^CLEANFILES = .*|CLEANFILES = |' \
	    -e 's|^Makefile: |Makefile.old: |' \
	    -e 's|^VPATH = .*|VPATH = .|' > $@

Makefile.repository: Makefile
	cat $< | \
	sed -e 's|^srcdir = .*|srcdir = '`cd $(srcdir) && pwd`'|' \
	    -e 's|^top_srcdir = .*|top_srcdir = '`cd $(top_srcdir) && pwd`'|' \
	    -e 's|^top_builddir = .*|top_builddir = '`cd $(top_builddir) && pwd`'|' \
	    -e 's|^pkgdata_DATA = .*|pkgdata_DATA = |' \
	    -e 's|^CLEANFILES = .*|CLEANFILES = make.out|' \
	    -e 's|^lib_LTLIBRARIES = .*|lib_LTLIBRARIES = |' \
	    -e 's|^BUILT_SOURCES = .*|BUILT_SOURCES = |' \
	    -e 's/^SUBDIRS = .*/SUBDIRS = $$(shell ls | grep -v Makefile)/' \
	    -e 's|^Makefile: |Makefile.old: |' \
	    -e 's|^VPATH = .*|VPATH = .|' | grep -v '^include ' > $@
