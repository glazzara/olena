// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/*!
 *  \file   tests/util/tree_to_fast.cc
 *
 *  \brief  test of mln::util::tree_to_fast
 *
 */

#include <mln/util/tree.hh>
#include <mln/util/tree_to_fast.hh>
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

  util::node<unsigned> node(elt1);
  util::node<unsigned>* node2 =  node.add_child(elt2);
  node.add_child(elt3);
  mln_assertion(node2);
  node2->add_child(elt4);
  node2->add_child(elt5);
  util::node<unsigned>* node3 = node.search(elt4);
  mln_assertion(node3);
  node3 = node2->search(elt1);
  mln_assertion(!node3);
  util::tree<unsigned>* tre = new util::tree<unsigned>(&node);
  mln_assertion(tre);
  tre->add_tree_up(elt6);
  mln_assertion (tre->check_consistency());


  util::tree_fast<unsigned> tree_fast = util::tree_to_fast(*tre);
  mln_assertion(tree_fast.has (elt1));
  mln_assertion(tree_fast.has (elt2));
  mln_assertion(tree_fast.has (elt3));
  mln_assertion(tree_fast.has (elt4));
  mln_assertion(tree_fast.has (elt5));
  mln_assertion(tree_fast.has (elt6));
  mln_assertion(tree_fast.search(elt6) == tree_fast.root_);
}
