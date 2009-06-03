// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file tests/border/equalize_full.cc
 *
 * \brief Tests on mln::border::equalize.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/border/get.hh>
#include <mln/border/equalize.hh>

int main()
{
  using namespace mln;
  unsigned sli = 5;
  unsigned row = 10;
  unsigned col = 20;
  unsigned border1 = 42;
  unsigned border2 = 36;
  unsigned new_border = 51;



  {
    typedef image1d<int> I;
    (std::cerr << "Tests border::equalize in 1d ... ").flush ();
    I ima1(col, border1);
    I ima2(col, border2);
    border::equalize(ima1, ima2, new_border);
    mln_assertion(border::get(ima1) == new_border);
    mln_assertion(border::get(ima2) == new_border);
    std::cerr << "OK" << std::endl;
  }

  {
    typedef image2d<int> I;
    (std::cerr << "Tests border::equalize in 2d ... ").flush ();
    I ima1(row, col, border1);
    I ima2(row, col, border2);
    border::equalize(ima1, ima2, new_border);
    mln_assertion(border::get(ima1) == new_border);
    mln_assertion(border::get(ima2) == new_border);
    std::cerr << "OK" << std::endl;
  }

  {
    typedef image3d<int> I;
    (std::cerr << "Tests border::equalize in 3d ... ").flush ();
    I ima1(sli, row, col, border1);
    I ima2(sli, row, col, border2);
    border::equalize(ima1, ima2, new_border);
    mln_assertion(border::get(ima1) == new_border);
    mln_assertion(border::get(ima2) == new_border);
    std::cerr << "OK" << std::endl;
  }

}
