// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/util/tree.hh>

int main ()
{
  using namespace mln;

  unsigned elt1 =  1;
  unsigned elt2 =  2;
  unsigned elt3 =  3;
  unsigned elt4 =  4;
  unsigned elt5 =  5;
  unsigned elt6 = 42;

  util::tree_node<unsigned> node(elt1);
  util::tree_node<unsigned>* node2 =  node.add_child(elt2);
  node.add_child(elt3);
  mln_assertion(node2);
  node2->add_child(elt4);
  node2->add_child(elt5);
  util::tree_node<unsigned>* node3 = node.search(elt4);
  mln_assertion(node3);
  node3 = node2->search(elt1);
  mln_assertion(!node3);
  util::tree<unsigned>* tre = new util::tree<unsigned>(&node);
  mln_assertion(tre);
  tre->add_tree_up(elt6);
  mln_assertion (tre->check_consistency());
}
