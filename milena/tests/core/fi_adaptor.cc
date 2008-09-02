// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/fi_adaptor.cc
/// \brief Test on mln::fi_adaptor.

#include <mln/core/fi_adaptor.hh>
#include <mln/core/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/level/paste.hh>
#include <mln/level/median.hh>

#include <mln/display/show.hh>
#include <mln/display/save.hh>

#include "tests/data.hh"


using namespace mln;

using namespace mln::value;

int main()
{
  using typename value::int_u8;
  using typename value::rgb8;

  {
    /* FIXME: Under GNU/Linux or under any *nix OS (i.e. under Unix or
       Mac OS X), you need to call FreeImage_Initialise at the
       beginning of your main function and you need to call
       FreeImage_DeInitialise at the end of this main function (this
       is not needed when using FreeImage as a shared library
       (*.so)).  */
//     FreeImage_Initialise();

    win::rectangle2d rect(3, 3);

    fi_adaptor< image2d< int_u<8> > > adaptor;

    adaptor.load(MLN_IMG_DIR "/lena.pgm");
    display::save (adaptor);
    // FIXME: Don't use display::show in batch tests.
//     display::show (adaptor, "xv");

    image2d<int_u8> ima(adaptor.domain());

    level::median(adaptor, rect, ima);

    level::paste(ima, adaptor);

    display::save (adaptor);
    // FIXME: Don't use display::show in batch tests.
//     display::show (adaptor, "xv");

    // FIXME: Likewise.
    /* Check if this statement holds in our case.  Maybe we need to
       involve Libtool in to handle FreeImagePlus properly.  */
//     FreeImage_DeInitialise();
  }
}
