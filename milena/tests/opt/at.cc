// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/vmorph/cast_image.hh>

/*#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/pw/image.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/extension_val.hh>*/

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/compare.hh>

#include <mln/opt/at.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;
  const unsigned size = 50;

  /// One dimensions tests...
  {
    image1d<short> ima(size);
    debug::iota(ima);
    const image1d<short> cima = ima;

    point1d p(5);
    mln_assertion(cima(p) == opt::at(cima, 5));

    opt::at(ima, 5) = 12;
    mln_assertion(cima(p) == 12);
  }
  {
    typedef image1d<short> I;
    typedef sub_image< image1d<short>, box1d > II;

    I ima(size);
    II sub_ima(ima, make::box1d(4, 10));
    const II csub_ima(ima, make::box1d(4, 10));
    point1d p(5);

    data::fill(ima, short(51));
    mln_assertion(csub_ima(p) == opt::at(csub_ima, 5));
    opt::at(sub_ima, 5) = 12;
    mln_assertion(sub_ima(p) == 12);
  }
  {
    typedef image1d<unsigned short> I;
    typedef cast_image_<int, I> II;

    I in(size, size);
    II cast(in);
    const II ccast(in);
    point1d p(5);

    data::fill(in, (unsigned short)51);
    mln_assertion(ccast(p) == opt::at(ccast, 5));
    // FIXME
    //opt::at(cast, 5) = 12;
    //mln_assertion(cast(p) == 12);
  }

  /// Two dimensions tests...
  {
    image2d<short> ima(size, size);
    debug::iota(ima);
    const image2d<short> cima = ima;

    point2d p(5, 5);
    mln_assertion(cima(p) == opt::at(cima, 5, 5));

    opt::at(ima, 5, 5) = 12;
    mln_assertion(cima(p) == 12);
  }
  {
    typedef image2d<short> I;
    typedef sub_image< image2d<short>, box2d > II;

    I ima(size, size);
    II sub_ima(ima, make::box2d(4,4, 10, 10));
    const II csub_ima(ima, make::box2d(4, 4, 10, 10));
    point2d p(5, 5);

    data::fill(ima, short(51));
    mln_assertion(csub_ima(p) == opt::at(csub_ima, 5, 5));
    opt::at(sub_ima, 5, 5) = 12;
    mln_assertion(sub_ima(p) == 12);
  }
  {
    typedef image2d<unsigned short> I;
    typedef cast_image_<int, I> II;

    I in(size, size);
    II cast(in);
    const II ccast(in);
    point2d p(5,5);

    data::fill(in, (unsigned short)51);
    mln_assertion(ccast(p) == opt::at(ccast, 5, 5));
    // FIXME
    //opt::at(cast, 5) = 12;
    //mln_assertion(cast(p) == 12);
  }


   /// Three dimensions tests...
  {
    image3d<short> ima(size, size, size);
    debug::iota(ima);
    const image3d<short> cima = ima;

    point3d p(5, 5, 5);
    mln_assertion(cima(p) == opt::at(cima, 5, 5, 5));

    opt::at(ima, 5, 5, 5) = 12;
    mln_assertion(cima(p) == 12);
  }
}
