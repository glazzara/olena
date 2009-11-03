# Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE).
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

SUBDIRS = . wrappers

include_HEADERS = dyn-all.hh dyn-light.hh
lib_LTLIBRARIES = libdyn.la

ERB = ruby $(top_srcdir)/config/erbx

dyn_light_hh_deps =				\
  $(srcdir)/name_of.hh				\
  function.hh					\
  all_methods.hh				\
  $(srcdir)/data.hh				\
  $(srcdir)/policy.hh

dyn_all_hh_deps =				\
  config.hh					\
  $(srcdir)/name_of.hh				\
  function.hh					\
  all_methods.hh				\
  $(srcdir)/data.hh				\
  $(srcdir)/data.hxx				\
  $(srcdir)/function_loader.hh			\
  $(srcdir)/dyn.hh

# Ask Make to build these first since they are #include'd.
BUILT_SOURCES =					\
  all_methods.hh				\
  function.hh					\
  config.hh

# Clean generated files.
CLEANFILES =					\
  $(BUILT_SOURCES)				\
  dyn-all.hh					\
  dyn-light.hh					\
  all_methods.cc				\
  function.cc

libdyn_la_SOURCES =				\
  dyn.hh					\
  data.hh data.hxx data.cc			\
  all_methods.hh all_methods.cc			\
  function.hh function.cc			\
  function_loader.hh function_loader.cc		\
  name_of.hh name_of.cc				\
  policy.hh policy.cc				\
  ruby_stream.hh ruby_stream.cc			\
  ansidecl.h demangle.h config.hh

# Provide a dummy `environ' symbol expected by libiberty (from the
# MacPorts), but missing on Mac OS X.  This is just a workaround.  We
# should file a problem report to the MacPorts about this.
if DARWIN
  libdyn_la_SOURCES += libiberty-fix.c
endif DARWIN

libdyn_la_LIBADD = @RUBY_LIBRUBYARG_SHARED@ $(LIBLTDL) \
  $(top_builddir)/libmd5/libmd5.la $(BOOST_FILESYSTEM_LIBS)
libdyn_la_DEPENDENCIES = $(LTDLDEPS)

libdyn_la_CPPFLAGS =							   \
  $(LTDLINCL) $(BOOST_CPPFLAGS)						   \
  -I$(top_builddir) -I$(srcdir) -I. -I$(top_srcdir)/libmd5 -I@RUBY_topdir@
libdyn_la_CXXFLAGS = $(WARNING_CXXFLAGS)
libdyn_la_LDFLAGS = -version-info 0:1:0 $(BOOST_FILESYSTEM_LDFLAGS)


#############
### RULES ###
#############

cut_local_includes = grep -v '^.[\t ]*include[\t ]*".*"'

dyn-all.hh: $(dyn_all_hh_deps)
	rm -f $@
	echo "// Generated: do not edit by hand!" > $@
	echo "#define DYN_FULL_IMPLEMENTATION" >> $@
	cat $(dyn_all_hh_deps) | $(cut_local_includes) >> $@
	chmod -w $@

dyn-light.hh: $(dyn_light_hh_deps)
	rm -f $@
	echo "// Generated: do not edit by hand!" > $@
	cat $(dyn_light_hh_deps) | $(cut_local_includes) >> $@
	chmod -w $@

%.hh: %.erb.hh
	$(ERB) -i $< -o $@

%.cc: %.erb.cc
	$(ERB) -i $< -o $@
