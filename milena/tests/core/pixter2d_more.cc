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

/*! \file tests/pixter2d.cc
 *
 * \brief Tests on mln::fwd_pixter2d.
 */

#include <mln/core/image2d.hh>


const unsigned size = 20;
const int v = 51;


template <typename I>
void test_fill(I& ima)
{
  mln_pixter(I) pxl(ima);
  unsigned i = 0;
  for_all(pxl)
    {
      ++i;
      pxl.val() = v;
    }
  mln_assertion(i == size * size);
  mln_assertion(! pxl.is_valid());

  mln_piter(I) p(ima.domain());
  for_all(p)
    mln_assertion(ima(p) == v);
}


template <typename I>
void test_const(const I& imac, I& ima)
{
  {
    mln_pixter(const I) pxl(imac); // const is mandatory
    pxl.start();
    mln_assertion(pxl.val() == v);
    // pxl.val() = v; // error is OK since pixter on 'const I' 
  }
  {
    // mln_pixter(I) pxl_(imac); // error is OK since mutable I but const imac
    mln_pixter(I) pxl(ima);
    pxl.start();
    pxl.val() = 2 * pxl.val();
    mln_assertion(pxl.val() == 2 * v);
  }
  {
    mln_pixter(const I) pxl(ima); // const promotion is OK
    pxl.start();
    mln_assertion(pxl.val() == 2 * v);
    // pxl.val() = v; // error is OK since pixter on 'const I' 
  }
}



int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(size, size);

  test_fill(ima);
  test_const(ima, ima);
}
