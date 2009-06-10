// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/util/tree_fast.hh>

int main ()
{
  using namespace mln;

  unsigned elt1 = 1;
  unsigned elt2 = 2;
  unsigned elt3 = 3;
  unsigned elt4 = 4;
  unsigned elt5 = 5;
  unsigned elt6= 42;

  util::tree_fast<unsigned> tree_fast(elt1);
  mln_assertion(tree_fast.has (elt1));
  tree_fast.add_child(tree_fast.search(elt1), elt2);
  mln_assertion(tree_fast.has (elt2));
  tree_fast.add_child(tree_fast.search(elt1), elt3);
  mln_assertion(tree_fast.has (elt3));
  tree_fast.add_child(tree_fast.search(elt2), elt4);
  mln_assertion(tree_fast.has (elt4));
  tree_fast.add_child(tree_fast.search(elt2), elt5);
  mln_assertion(tree_fast.has (elt5));
  tree_fast.add_parent(elt6);
  mln_assertion(tree_fast.has (elt6));
  mln_assertion(tree_fast.search(elt6) == tree_fast.root_);
}
