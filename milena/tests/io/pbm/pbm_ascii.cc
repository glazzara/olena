// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/data/compare.hh>

#include <mln/literal/colors.hh>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/make/box2d.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<bool> lena_raw;
  io::pbm::load(lena_raw, MLN_TESTS_IMG_DIR "/lena_raw.pbm");

  {
    image2d<bool> lena_ascii;
    io::pbm::load(lena_ascii, MLN_TESTS_IMG_DIR "/lena_ascii.pbm");

    mln_assertion(lena_raw == lena_ascii);
  }

  {
    image2d<bool> lena_ascii;
    io::pbm::load(lena_ascii, MLN_TESTS_IMG_DIR "/lena_ascii.pbm");
    io::pbm::save(lena_ascii, "pbm_ascii-out.pbm");

    image2d<bool> out;
    io::pbm::load(out, "pbm_ascii-out.pbm");

    mln_assertion(out == lena_ascii);
    mln_assertion(out == lena_raw);
  }

}
