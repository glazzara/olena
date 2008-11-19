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

/// \file tests/morpho/erosion.cc
///
/// Test on mln::morpho::erosion.

#include <mln/core/image/image2d.hh>
#include <mln/win/all.hh>

#include <mln/debug/iota.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/morpho/erosion.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 0;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  win::rectangle2d rec(21, 21);
  win::hline2d hline(31);
  win::vline2d vline(31);
  win::diag2d diag2d(31);
  win::backdiag2d backdiag2d(31);
  win::octagon2d oct(6 * 3 + 1);
  image2d<int_u8> out;
  image2d<int_u8> ref;

//   trace::quiet = false;


  // Rectangle

  {
    ref = morpho::impl::generic::erosion_on_function(lena, rec);
  }

  {
    out = morpho::erosion(lena, rec);
    bool test = out == ref;
    mln_assertion(test);
  }

  {
    out = morpho::impl::erosion_arbitrary_2d(lena, rec);
    bool test = out == ref;
    mln_assertion(test);
  }


  // Hline

  {
    ref = morpho::impl::generic::erosion_on_function(lena, hline);
  }

  {
    out = morpho::erosion(lena, hline);
    bool test = out == ref;
    mln_assertion(test);
  }

  {
    out = morpho::impl::erosion_arbitrary_2d(lena, hline);
    bool test = out == ref;
    mln_assertion(test);
  }


  // Vline

  {
    ref = morpho::impl::generic::erosion_on_function(lena, vline);
  }

  {
    out = morpho::erosion(lena, vline);
    bool test = out == ref;
    mln_assertion(test);
  }

  {
    out = morpho::impl::erosion_arbitrary_2d(lena, vline);
    bool test = out == ref;
    mln_assertion(test);
  }


  // Diag2d

  {
    ref = morpho::impl::generic::erosion_on_function(lena, diag2d);
  }

  {
    out = morpho::erosion(lena, diag2d);
    bool test = out == ref;
    mln_assertion(test);
  }

  {
    out = morpho::impl::erosion_arbitrary_2d(lena, diag2d);
    bool test = out == ref;
    mln_assertion(test);
  }


  // Backdiag2d

  {
    ref = morpho::impl::generic::erosion_on_function(lena, backdiag2d);
  }

  {
    out = morpho::erosion(lena, backdiag2d);
    bool test = out == ref;
    mln_assertion(test);
  }

  {
    out = morpho::impl::erosion_arbitrary_2d(lena, backdiag2d);
    bool test = out == ref;
    mln_assertion(test);
  }



  // Octagon

  {
    ref = morpho::impl::generic::erosion_on_function(lena, oct);
    io::pgm::save(ref, "out_oct_ref.pgm");
  }

  {
    out = morpho::erosion(lena, oct);
    bool test = out == ref;
    mln_assertion(test);
  }

}
