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
/// Test on mln::morpho::top_hat.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/top_hat.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

//   trace::quiet = false;

  image2d<int_u8> ima(3, 3, 0);
  debug::iota(ima);
  debug::println(ima);

  debug::println( morpho::top_hat_white(ima, c4().win()) );
  debug::println( morpho::top_hat_black(ima, c4().win()) );
  debug::println( morpho::top_hat_self_complementary(ima, c4().win()) );

  image2d<bool> msk(3, 3, 0);
  data::fill(msk, pw::value(ima) >= pw::cst(5u));
  debug::println(msk);

  debug::println( morpho::top_hat_white(msk, c4().win()) );
  debug::println( morpho::top_hat_black(msk, c4().win()) );
  debug::println( morpho::top_hat_self_complementary(msk, c4().win()) );
}
