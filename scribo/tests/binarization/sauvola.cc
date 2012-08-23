// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/load.hh>

#include <scribo/binarization/sauvola.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;

  // even height and width
  {
    image2d<value::int_u8> input;
    io::pgm::load(input, MILENA_IMG_DIR "/lena.pgm");

    image2d<bool> bin = scribo::binarization::sauvola(input, 101);

    image2d<bool> ref;
    io::pbm::load(ref, SCRIBO_TESTS_DIR "binarization/sauvola.ref.pbm");

    mln_assertion(bin == ref);
  }

  // even height and odd width
  {
    image2d<value::int_u8> input;
    io::pgm::load(input, SCRIBO_IMG_DIR "/lena_wodd_heven.pgm");

    image2d<bool> bin = scribo::binarization::sauvola(input, 101);

    image2d<bool> ref;
    io::pbm::load(ref, SCRIBO_TESTS_DIR "binarization/sauvola_wodd_heven.ref.pbm");

    mln_assertion(bin == ref);
  }

  // odd height and even width
  {
    image2d<value::int_u8> input;
    io::pgm::load(input, SCRIBO_IMG_DIR "/lena_weven_hodd.pgm");

    image2d<bool> bin = scribo::binarization::sauvola(input, 101);

    image2d<bool> ref;
    io::pbm::load(ref, SCRIBO_TESTS_DIR "binarization/sauvola_weven_hodd.ref.pbm");

    mln_assertion(bin == ref);
  }

  // odd height and width
  {
    image2d<value::int_u8> input;
    io::pgm::load(input, SCRIBO_IMG_DIR "/lena_wodd_hodd.pgm");

    image2d<bool> bin = scribo::binarization::sauvola(input, 101);

    image2d<bool> ref;
    io::pbm::load(ref, SCRIBO_TESTS_DIR "binarization/sauvola_wodd_hodd.ref.pbm");

    mln_assertion(bin == ref);
  }
}
