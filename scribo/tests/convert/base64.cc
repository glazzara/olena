// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE).
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

/// \file

#include <cstdio>
#include <cstdlib>

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>

#include <mln/value/int_u.hh>
#include <mln/value/int_u8.hh>

#include <mln/debug/iota.hh>

#include <mln/value/int_u16.hh>

#include <scribo/convert/to_base64.hh>
#include <scribo/convert/from_base64.hh>

#include "tests/data.hh"

using namespace mln;

int main()
{

  // PBM
  {
    image2d<bool> ima;
    io::pbm::load(ima, SCRIBO_IMG_DIR "/wildly.pbm");

    util::array<unsigned char> out64;
    scribo::convert::to_base64(ima, out64);

    image2d<bool> outbin(ima.domain());
    scribo::convert::from_base64(out64, outbin);

    mln_assertion(outbin == ima);
  }

  // PGM (8bits)
  {
    image2d<value::int_u8> ima;
    io::pgm::load(ima, SCRIBO_IMG_DIR "/text_to_group.pgm");

    util::array<unsigned char> out64;
    scribo::convert::to_base64(ima, out64);

    image2d<value::int_u8> outbin(ima.domain());
    scribo::convert::from_base64(out64, outbin);

    mln_assertion(outbin == ima);
  }

  // PGM (30bits)
  {
    image2d<value::int_u<30> > ima(3, 3);
    debug::iota(ima, 100000);

    util::array<unsigned char> out64;
    scribo::convert::to_base64(ima, out64);

    image2d<value::int_u<30> > outbin(ima.domain());
    scribo::convert::from_base64(out64, outbin);

    mln_assertion(outbin == ima);
  }
}
