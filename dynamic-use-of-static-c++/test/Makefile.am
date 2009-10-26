AM_CPPFLAGS = -I$(srcdir)/fixtures -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CXXFLAGS = $(WARNING_CXXFLAGS)

SUBDIRS = vaucanson

#erb# =begin # Read the documentation of erbx to see how to change this part
#erb# <%
#erb# TESTS = %w[
#erb#   function_loader
#erb#   damien
#erb#   containers
#erb#   var_and_val
#erb#   methods
#erb#   olena
#erb# ]
#erb# %>
#erb# check_PROGRAMS = <%= TESTS.map { |t| "#{t}.test" }.join ' ' %>
#erb# <% TESTS.each do |name| %>
#erb# <%= name %>_test_SOURCES = test_<%= name %>.cc
#erb# <%= name %>_test_LDADD = $(top_builddir)/src/libdyn.la
#erb# <%= name %>_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la
#erb# <% end %>
#erb# =end

#erb# =generated

check_PROGRAMS = function_loader.test damien.test containers.test var_and_val.test methods.test olena.test

function_loader_test_SOURCES = test_function_loader.cc
function_loader_test_LDADD = $(top_builddir)/src/libdyn.la
function_loader_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

damien_test_SOURCES = test_damien.cc
damien_test_LDADD = $(top_builddir)/src/libdyn.la
damien_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

containers_test_SOURCES = test_containers.cc
containers_test_LDADD = $(top_builddir)/src/libdyn.la
containers_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

var_and_val_test_SOURCES = test_var_and_val.cc
var_and_val_test_LDADD = $(top_builddir)/src/libdyn.la
var_and_val_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

methods_test_SOURCES = test_methods.cc
methods_test_LDADD = $(top_builddir)/src/libdyn.la
methods_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

olena_test_SOURCES = test_olena.cc
olena_test_LDADD = $(top_builddir)/src/libdyn.la
olena_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

#erb# =end_generated

TESTS = $(check_PROGRAMS)

tests: $(check_PROGRAMS)

repository_clean: clean
	rm -rf repository
