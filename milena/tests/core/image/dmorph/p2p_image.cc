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

/// \file tests/core/image/dmorph/p2p_image.cc
///
/// Tests on mln::p2p_image.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/p2p_image.hh>
#include <mln/fun/p2p/translation.hh>



# define ima_  apply_p2p(ima, fun::p2p::translation(dp))


int main()
{
  using namespace mln;

  box2d b = make::box2d(0,0, 2,2);
  image2d<int> ima(b, 0); // No border.

  /// Initialize image with '2'. Do not rely on data::fill.
  mln_piter_(image2d<int>) p(ima.domain());
  for_all(p)
    ima(p) = 2;
  /// Set special values.
  ima(point2d(0,0)) = 1;
  ima(point2d(2,2)) = 9;

  dpoint2d dp(-1,+1);
  box2d b_ = make::box2d(-1,+1, 1,3);

  mln_assertion( ima_.domain() == b_ );

  mln_assertion( ima_(point2d(-1,+1)) == 1 );
  mln_assertion( ima_(point2d( 1, 3)) == 9 );
}
