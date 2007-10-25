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
 *  \file   tests/tree_fast.cc
 *
 *  \brief  test of mln::util::tree_fast
 *
 */

#include <mln/util/tree_fast.hh>
#include <mln/core/contract.hh>
#include <iostream>

int main (void)
{
  using namespace mln;

  unsigned elt1 = 1;
  unsigned elt2 = 2;
  unsigned elt3 = 3;
  unsigned elt4 = 4;
  unsigned elt5 = 5;
  unsigned elt6= 42;

  util::tree<unsigned> tree(elt1);
  mln_assertion(tree.has (elt1));
  tree.add_child(tree.search(elt1), elt2);
  mln_assertion(tree.has (elt2));
  tree.add_child(tree.search(elt1), elt3);
  mln_assertion(tree.has (elt3));
  tree.add_child(tree.search(elt2), elt4);
  mln_assertion(tree.has (elt4));
  tree.add_child(tree.search(elt2), elt5);
  mln_assertion(tree.has (elt5));
  tree.add_parent(elt6);
  mln_assertion(tree.has (elt6));
  mln_assertion(tree.search(elt6) == tree.root_);
}
