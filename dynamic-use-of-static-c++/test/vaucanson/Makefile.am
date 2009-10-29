# Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE).
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

# FIXME: Clean me!

check-local_DISABLE: tree.yml
	@@BIN@@/yaml_to_dyn_decls < $<
	cp dyn_vaucanson_dyn_mirror.hh @@VCSN@@/src/demos/function_library
	wc tree.swig tree.yml dyn_vaucanson_dyn_mirror.hh dyn_vaucanson_methods.yml

vcsn.i: @@BIN@@/mk_swig_input
	@@BIN@@/mk_swig_input @@VCSN@@/include

tree.swig: vcsn.i
	-@@SWIG@@ @@SWIGINCLUDES@@ -I@@VCSN@@/include -c++ -tree-dumper $< > $@

tree.yml: tree.swig
	@@BIN@@/swig_tree_to_yaml < $< > $@

clean-local:
	rm -f tree.swig tree.yml vcsn.i dyn_vaucanson_methods.hh dyn_vaucanson_dyn_mirror.hh
