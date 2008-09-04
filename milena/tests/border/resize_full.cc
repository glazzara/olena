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

/*! \file tests/border/resize_full.cc
 *
 * \brief Tests on mln::border::resize.
 */


#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/border/resize.hh>
#include <mln/border/get.hh>

using namespace mln;

int
main (void)
{
  unsigned sli = 51;
  unsigned row = 42;
  unsigned col = 100;



  unsigned border1 = 1;
  unsigned border2 = 3;
  unsigned border3 = 42;
  unsigned border4 = 51;
  unsigned border5 = 36;
  unsigned border6 = 2;
  unsigned border7 = 0;
  unsigned border8 = 8;
  unsigned border9 = 1;


  {
    (std::cerr << "Test border::resize in 1d ... ").flush ();
    image1d<value::int_u8> ima(col, border1);

    mln_assertion(border::get(ima) == border1);
    border::resize (ima, border2);
    mln_assertion(border::get(ima) == border2);
    border::resize (ima, border3);
    mln_assertion(border::get(ima) == border3);
    border::resize (ima, border4);
    mln_assertion(border::get(ima) == border4);
    border::resize (ima, border5);
    mln_assertion(border::get(ima) == border5);
    border::resize (ima, border6);
    mln_assertion(border::get(ima) == border6);
    border::resize (ima, border7);
    mln_assertion(border::get(ima) == border7);
    border::resize (ima, border8);
    mln_assertion(border::get(ima) == border8);
    border::resize (ima, border9);
    mln_assertion(border::get(ima) == border9);
    std::cerr << "OK" << std::endl;
  }

  {
    (std::cerr << "Test border::resize in 2d ... ").flush ();
    image2d<value::int_u8> ima(row, col, border1);

    mln_assertion(border::get(ima) == border1);
    border::resize (ima, border2);
    mln_assertion(border::get(ima) == border2);
    border::resize (ima, border3);
    mln_assertion(border::get(ima) == border3);
    border::resize (ima, border4);
    mln_assertion(border::get(ima) == border4);
    border::resize (ima, border5);
    mln_assertion(border::get(ima) == border5);
    border::resize (ima, border6);
    mln_assertion(border::get(ima) == border6);
    border::resize (ima, border7);
    mln_assertion(border::get(ima) == border7);
    border::resize (ima, border8);
    mln_assertion(border::get(ima) == border8);
    border::resize (ima, border9);
    mln_assertion(border::get(ima) == border9);
    std::cerr << "OK" << std::endl;
  }

  {
    (std::cerr << "Test border::resize in 3d ... ").flush ();
    image3d<value::int_u8> ima(sli, row, col, border1);

    mln_assertion(border::get(ima) == border1);
    border::resize (ima, border2);
    mln_assertion(border::get(ima) == border2);
    border::resize (ima, border3);
    mln_assertion(border::get(ima) == border3);
    border::resize (ima, border4);
    mln_assertion(border::get(ima) == border4);
    border::resize (ima, border5);
    mln_assertion(border::get(ima) == border5);
    border::resize (ima, border6);
    mln_assertion(border::get(ima) == border6);
    border::resize (ima, border7);
    mln_assertion(border::get(ima) == border7);
    border::resize (ima, border8);
    mln_assertion(border::get(ima) == border8);
    border::resize (ima, border9);
    mln_assertion(border::get(ima) == border9);
    std::cerr << "OK" << std::endl;
  }

}
