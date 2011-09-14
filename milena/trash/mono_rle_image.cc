// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/core/image/mono_rle_image.cc
 *
 * \brief Test on mln::labeling::blobs.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/pw/all.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/data/transform.hh>
#include <mln/data/paste.hh>
#include <mln/data/compare.hh>
#include <mln/io/pgm/save.hh>

#include <mln/core/image/mono_rle_encode.hh>

#include "tests/data.hh"


struct fold_t : public mln::Function_v2v< fold_t >
{
  typedef mln::value::int_u8 result;
  result operator()(unsigned i) const { return i == 0 ? 0 : (i - 1) % 255 + 1; }
};

struct only_two_t : public mln::Function_v2v< only_two_t >
{
  typedef mln::value::int_u8 result;
  result operator()(unsigned i) const { return i == 2 ? 2 : 0; }
};


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/small.pgm");
  image2d<int_u8> cmp(lena.domain());

  unsigned n;
  image2d<unsigned> labels =
    labeling::blobs((pw::value(lena) > pw::cst(172u)) | lena.domain(),
		    c4(), n);

  mono_rle_image<point2d, int_u8> rle =
    mono_rle_encode(data::transform(labels, fold_t()), 2);

  std::cout << n << ", compression ratio: " << rle.compression() << std::endl;

  data::fill(cmp, literal::zero);
  data::paste(rle, cmp);

  mln_assertion(cmp == data::transform(labels, only_two_t()));
}
