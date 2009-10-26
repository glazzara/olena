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
