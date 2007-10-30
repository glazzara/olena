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

/*! \file tests/fi_adaptor.cc
 *
 * \brief Test on mln::fi_adaptor.
 */

#include <mln/core/fi_adaptor.hh>
#include <mln/core/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/level/paste.hh>
#include <mln/level/median.hh>

#include <mln/display/show.hh>
#include <mln/display/save.hh>

#include <FreeImagePlus.h>


using namespace mln;

int main()
{
  using typename value::int_u8;
  using typename value::rgb8;

  // FIXME: Is this necessary?  See FreeImagePlus' documentation.
  FreeImage_Initialise();

  win::rectangle2d rect(51, 51);

  fi_adaptor< image2d<int_u8> > adaptor;

  adaptor.load("../img/lena.pgm");

  image2d<int_u8> ima(adaptor.domain());

  level::median(adaptor, rect, ima);

  level::paste(ima, adaptor);

  display::save (adaptor);
  display::show (adaptor, "xv");

  // FIXME: Likewise.
  /* A comment here (probably coming from an example of FreeImagePlus'
     documentation) used to say:

       call this ONLY when linking with FreeImage as a static library

     Check if this statement holds in our case.  Maybe we need to
     involve Libtool in to handle FreeImagePlus properly.  */
  FreeImage_DeInitialise();
}
