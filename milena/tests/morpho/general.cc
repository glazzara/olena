// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/morpho/general.cc
///
/// Test on mln::morpho::general.

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/load.hh>

#include <mln/win/all.hh>
#include <mln/morpho/erosion.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;


  // Windows.
  win::rectangle2d rec(5, 5);
  win::hline2d     hline(7);
  win::vline2d     vline(7);
  win::diag2d      diag2d(7);
  win::backdiag2d  backdiag2d(7);
  win::octagon2d   oct(2 * 3 + 1);

  morpho::erosion_op ero;

  {
    image2d<int_u8> lena, out, ref;
    io::pgm::load(lena, MLN_IMG_DIR "/tiny.pgm");


    // Rectangle.

    ref = morpho::impl::generic::general_on_function(ero, lena, rec);

    out = morpho::impl::general_rectangle2d(ero, lena, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_arbitrary_2d(ero, lena, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_directional(ero, lena, rec, 0);
    mln_assertion(out == ref);

    out = morpho::impl::general_directional(ero, lena, rec, 1);
    mln_assertion(out == ref);

    /*

    // Hline.
    ref = morpho::impl::generic::general_on_function(ero, lena, hline);

    // Vline.
    ref = morpho::impl::generic::general_on_function(ero, lena, vline);

    // Diag2d.
    ref = morpho::impl::generic::erosion_on_function(lena, diag2d);

    // Backdiag2d.
    ref = morpho::impl::generic::erosion_on_function(lena, backdiag2d);

    // Octagon
    ref = morpho::impl::generic::erosion_on_function(lena, oct);
    // io::pgm::save(ref, "out_oct_ref.pgm");

    */
  }


  /*

  {
    trace::quiet = false;

    image2d<bool> pic;
    io::pbm::load(pic, MLN_IMG_DIR "/tiny.pbm");

    morpho::general(ero, pic, win::rectangle2d(3,3));

    trace::quiet = true;
  }

  */


  {
    image2d<bool> pic, out, ref;
    io::pbm::load(pic, MLN_IMG_DIR "/tiny.pbm");

    // Rectangle.

    ref = morpho::impl::generic::general_on_set(ero, pic, rec);

    out = morpho::impl::general_on_set_centered(ero, pic, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_on_set_centered_fastest(ero, pic, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_rectangle2d(ero, pic, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_arbitrary_2d(ero, pic, rec);
    mln_assertion(out == ref);

    out = morpho::impl::general_directional(ero, pic, rec, 0);
    mln_assertion(out == ref);

    out = morpho::impl::general_directional(ero, pic, rec, 1);
    mln_assertion(out == ref);
  }

}
