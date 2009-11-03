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

AM_CPPFLAGS = -I$(srcdir)/fixtures -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CXXFLAGS = $(WARNING_CXXFLAGS)

SUBDIRS = . vaucanson wrappers

#erb# =begin # Read the documentation of erbx to see how to change this part
#erb# <%
#erb# TESTS = %w[
#erb#   function_loader
#erb#   damien
#erb#   containers
#erb#   var_and_val
#erb#   methods
#erb#   milena
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

check_PROGRAMS = function_loader.test damien.test containers.test var_and_val.test methods.test milena.test

# FIXME: Factor *_LDADD variables.
# FIXME: *_DEPENDENCIES are probably useless.
# FIXME: Use Automake 1.11's default extension for tests?
# FIXME: If the code reduces to a single definition of
# `check_PROGRAM', stop using erbx.

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

milena_test_SOURCES = test_milena.cc
milena_test_LDADD = $(top_builddir)/src/libdyn.la
milena_test_DEPENDENCIES = $(top_builddir)/src/libdyn.la

#erb# =end_generated

TESTS = $(check_PROGRAMS)

tests: $(check_PROGRAMS)

clean-local: clean-repository
.PHONY: clean-repository
clean-repository:
	-rm -rf repository
