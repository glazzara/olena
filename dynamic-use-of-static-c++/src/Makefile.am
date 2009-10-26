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

libdyn_la_LIBADD = @LIBERTYARG@ @RUBY_LIBRUBYARG_SHARED@ $(LIBLTDL)
libdyn_la_DEPENDENCIES = $(LTDLDEPS)

libdyn_la_CPPFLAGS =							\
	$(LTDLINCL) -I$(top_builddir) -I$(srcdir) -I. -I@RUBY_topdir@
libdyn_la_CXXFLAGS = $(WARNING_CXXFLAGS)
libdyn_la_LDFLAGS = -version-info 0:1:0


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
