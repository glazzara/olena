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

/*! \file tests/sparse_image.cc
 *
 * \brief Tests on mln::sparse_image.
 */
#include <mln/core/image2d.hh>
#include <mln/core/sparse_image.hh>
#include <mln/core/sparse_encode.hh>
#include <vector>

using namespace mln;


template <typename Pset>
void test(const Pset& my_set)
{
  typename Pset::fwd_piter run (my_set);
  for (run.start(); run.is_valid(); run.next())
    ;
  //std::cout << run << std::endl;

    //  std::cout << "Reverse" << std::endl;
  typename Pset::bkd_piter run2 (my_set);

  //std::cout << "Reverse\n";

  for (run2.start(); run2.is_valid(); run2.next())
    ;
  //   std::cout << run2 << std::endl;
}

template <typename I>
void test2(I& ima)
{
  typename I::fwd_piter run (ima.domain());
  for_all(run)
    ++ima(run);

  typename I::bkd_piter run2 (ima.domain());
  for_all(run2)
    assert(ima(run2) == 1);
}

int
main()
{
  mln::point2d p, q, r;
  p = mln::make::point2d(0, 1);
  q = mln::make::point2d(3, 0);
  r = mln::make::point2d(2, 2);
  mln::internal::run_pset_<mln::point2d> my_set;
  mln::sparse_image<mln::point2d, int> sparse;
  mln::sparse_image<mln::point2d, int> sparse2;


  my_set.insert(p, 5);
  my_set.insert(q, 8);
  test(my_set);

  std::vector<int> values;
  int a = 0;
  values.push_back(a);
  a = 0;
  values.push_back(a);
  a = 0;
  values.push_back(a);
  sparse.insert(q, 3, values);

  test(sparse.domain());
  test2(sparse);

   mln::image2d<int> ima2d (1, 5);

   ima2d(mln::make::point2d(0, 4)) = 5;
   ima2d(mln::make::point2d(0, 3)) = 2;

//   oln::debug::print(ima2d);

   sparse2 = sparse_encode(ima2d);
   

//   oln::debug::print(sparse2);
//   std::cout << std::endl;

  return 0;
}
