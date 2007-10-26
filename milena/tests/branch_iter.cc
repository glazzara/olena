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
 *  \file   tests/branch_iter.cc
 *
 *  \brief  test of mln::util::branch_iter
 *
 */

#include <mln/core/image2d.hh>
#include <mln/util/tree.hh>
#include <mln/util/branch_iter.hh>

int main()
{
  using namespace mln;

  util::node<int> n(11);

  util::tree<int> t(&n);

  util::node<int>* f = n.add_child(42);

  util::node<int>* g = f->add_child(421);
  util::node<int>* h = f->add_child(422);

  g->add_child(4211)->add_child(51)->add_child(52)->add_child(53)->add_child(54)->add_child(55);
  g->add_child(4212);

  f->add_child(4221);
  f->add_child(4222);

  n.add_child(43);

  util::branch<int> b(t, n);

  std::vector< util::node<int>* >::iterator it;
  util::branch_iter<int> p(b);
  for_all(p)
    {
      std::cout << "parcour : " << util::node<int>(p).elt() << std::endl;
    }
}
