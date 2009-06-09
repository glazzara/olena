// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file
///
/// Test on mln::morpho::elementary::gradient_internal.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>
#include <mln/value/int_u8.hh>
#include <mln/make/image2d.hh>

#include <mln/debug/iota.hh>

#include <mln/morpho/elementary/gradient_internal.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> ima(3, 3, 0);
  debug::iota(ima);

  {
    int_u8 ref_[9] = { 0, 1, 1,
		       3, 3, 3,
		       3, 3, 3 };
    image2d<int_u8> ref = make::image2d(ref_);

    image2d<int_u8> gra = morpho::elementary::gradient_internal(ima, c4());
    mln_assertion(gra == ref);
  }

  {
    bool ref_[9] = { 0, 0, 0,
		     0, 1, 1,
		     1, 0, 0 };
    image2d<bool> ref = make::image2d(ref_);

    image2d<bool> msk(3, 3, 0);
    data::fill(msk, pw::value(ima) >= pw::cst(5u));
    image2d<bool> gra = morpho::elementary::gradient_internal(msk, c4());
    mln_assertion(gra == ref);
  }

}
