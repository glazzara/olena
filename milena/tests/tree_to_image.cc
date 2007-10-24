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
 *  \file   tests/tree_to_image.cc
 *
 *  \brief  test of mln::util::tree_to_image
 *
 */

#include <mln/util/tree.hh>
#include <mln/core/contract.hh>
#include <mln/core/image2d.hh>
#include <mln/core/set_p.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/stretch.hh>
#include <mln/io/pgm/save.hh>
#include <vector>
#include <mln/util/tree_to_image.hh>

int main (void)
{
  using namespace mln;
  using value::int_u8;

  typedef set_p<point2d > I;

  image2d<int_u8> output (300, 300);

  I s1;
  I s2;
  I s3;
  I s4;
  I s5;
  I s6;
  I s7;

  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s1.insert(point2d(i, j));

  for (int i = 200; i < 300; ++i)
    for (int j = 0; j < 100; ++j)
      s2.insert(point2d(i, j));

  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s3.insert(point2d(i, j));

  for (int i = 260; i < 290; ++i)
    for (int j = 0; j < 50; ++j)
      s4.insert(point2d(i, j));

  for (int i = 200; i < 210; ++i)
    for (int j = 0; j < 50; ++j)
      s5.insert(point2d(i, j));

  for (int i = 270; i < 280; ++i)
    for (int j = 50; j < 60; ++j)
      s6.insert(point2d(i, j));

  for (int i = 0; i < 300; ++i)
    for (int j = 0; j < 200; ++j)
      s7.insert(point2d(i, j));

  util::node<I> node(s1);
  node.add_child(s2);
  node.add_child(s3);

  util::node<I>* node2 = node.search(s2);
  mln_assertion(node2);
  node2->add_child(s4);
  node2->add_child(s5);
  util::node<I>* node3 = node.search(s4);
  mln_assertion(node3);
  node3->add_child(s6);

  util::tree<I> tree (&node);

  tree.add_tree_up(s7);

  util::tree_to_image(tree, output);

  image2d<int_u8> out(output.domain());

  level::stretch (output, out);

  io::pgm::save(out, "out.pgm");

  std::cout << "out.pgm generate"
	    << std::endl;

//   tree3 = tree2->search(s1);
//   mln_assertion(!tree3);
}
