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

#include <mln/core/contract.hh>
#include <mln/core/image2d.hh>
#include <mln/core/set_p.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/stretch.hh>
#include <mln/io/pgm/save.hh>
#include <vector>
#include <mln/util/tree_fast_to_image.hh>
#include <mln/util/tree_fast.hh>

template <typename P, typename V>
struct fllt_node
{
  V	value;
  mln::set_p<P> points;
  mln::set_p<P> holes;
};


template <typename P, typename V>
bool operator==(const mln::set_p<P>& lhs, const mln::set_p<P>& rhs)
{
  std::size_t n = lhs.npoints ();
  mln::set_p<P> tmp;

  if (n != rhs.npoints ())
    return false;

//   for (std::size_t i = 0; i < n; ++i)
//     if ()

}

template <typename P, typename V>
bool operator==(const struct fllt_node<P,V>& lhs, const struct fllt_node<P,V>& rhs)
{
  if (lhs.value != rhs.value)
    return false;

  /// FIXME

  if (!(lhs.points == rhs.points))
    return false;

  if (!(lhs.holes == rhs.holes))
    return false;

  return true;
}

int main (void)
{
  using namespace mln;
  using value::int_u8;

  typedef set_p<point2d > I;
  typedef fllt_node<point2d, int_u8> T;

  T s1;
  T s2;
  T s3;
  T s4;
  T s5;
  T s6;
  T s7;

  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s1.points.insert(point2d(i, j));
  s1.value = 60;
  for (int i = 200; i < 300; ++i)
    for (int j = 0; j < 100; ++j)
      s2.points.insert(point2d(i, j));
  s2.value = 100;
  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 100; ++j)
      s3.points.insert(point2d(i, j));
  s3.value = 110;
  for (int i = 260; i < 290; ++i)
    for (int j = 0; j < 50; ++j)
      s4.points.insert(point2d(i, j));
  s4.value = 170;
  for (int i = 200; i < 210; ++i)
    for (int j = 0; j < 50; ++j)
      s5.points.insert(point2d(i, j));
  s5.value = 180;
  for (int i = 270; i < 280; ++i)
    for (int j = 50; j < 60; ++j)
      s6.points.insert(point2d(i, j));
  s6.value = 210;
  for (int i = 0; i < 300; ++i)
    for (int j = 0; j < 200; ++j)
      s7.points.insert(point2d(i, j));
  s7.value = 10;

  util::tree_fast<T> tree(s1);
  tree.add_child(tree.root_, s2);
  tree.add_child(tree.search(s1), s3);
  tree.add_child(tree.search(s2), s4);
  tree.add_child(tree.search(s2), s5);
  tree.add_child(tree.search(s4), s6);
  tree.add_parent(s7);
  image2d<int_u8> out (300,300);
  util::tree_fast_to_image(tree, out);
  io::pgm::save(out, "out.pgm");
  std::cout << "out.pgm generate"
	    << std::endl;
}
