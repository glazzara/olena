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
 *  \file   tests/tree.cc
 *
 *  \brief  test of mln::util::tree
 *
 */

#include <mln/util/tree.hh>
#include <mln/core/contract.hh>

int main (void)
{
  using namespace mln;

  unsigned elt1 = 1;
  unsigned elt2 = 2;
  unsigned elt3 = 3;
  unsigned elt4 = 4;
  unsigned elt5 = 5;

  util::tree<unsigned> tree(elt1);
  tree.add_child(elt2);
  tree.add_child(elt3);
  util::tree<unsigned>* tree2 = tree.search(elt2);
  mln_assertion(tree2);
  tree2->add_child(elt4);
  tree2->add_child(elt5);
  util::tree<unsigned>* tree3 = tree.search(elt4);
  mln_assertion(tree3);
  tree3 = tree2->search(elt1);
  mln_assertion(!tree3);
}
