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

/*! \file tests/core/image/rle_image.cc
 *
 * \brief Test on mln::core::value_enc_image.hh.
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/value_enc_image.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/pw/all.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/data/transform.hh>
#include <mln/data/paste.hh>
#include <mln/data/compare.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/alias/p_runs2d.hh>

#include "tests/data.hh"

#include <mln/core/image/value_encode.hh>

struct fold_t : public mln::Function_v2v< fold_t >
{

  typedef mln::value::int_u8 result;
  result operator()(unsigned i) const
  {
    return i == 0 ? 0 : (i - 1) % 255 + 1;
  }

};

int main()
{
  using namespace mln;

  /// Basic test
  {
    typedef value_enc_image<point2d, int> ima_type;

    p_runs2d pruns0;
    p_runs2d pruns1;
    p_runs2d pruns2;

    pruns0.insert(p_run<point2d>(point2d(0, 0), 2));

    pruns1.insert(p_run<point2d>(point2d(2, 4), 7));
    pruns1.insert(p_run<point2d>(point2d(18, 42), 5));
    pruns1.insert(p_run<point2d>(point2d(50, 76), 2));
    pruns1.insert(p_run<point2d>(point2d(17,40), 6));

    pruns2.insert(p_run<point2d>(point2d(10,10), 5));

    ima_type ima;
    ima.insert(pruns0, 0);
    ima.insert(pruns2, 2);
    ima.insert(pruns1, 1);

    mln_piter_(ima_type) piter (ima.domain());
    int i = 0;
    int nb = 0;
    for_all(piter)
    {
      assert(ima(piter) == i);

      ++nb;
      if (nb == 2)
	i = 2;
      if (nb == 7)
	i = 1;
    }
  }

  /// Basic test two
  {
    typedef value_enc_image<point2d, int> ima_type;
    ima_type ima;

     ima.insert(p_run<point2d>(point2d(0, 0), 2), 0);
     ima.insert(p_run<point2d>(point2d(10,10), 5), 2);

    ima.insert(p_run<point2d>(point2d(2, 4), 7), 1);
    ima.insert(p_run<point2d>(point2d(18, 42), 5), 1);
    ima.insert(p_run<point2d>(point2d(50, 76), 2), 1);
    ima.insert(p_run<point2d>(point2d(17,40), 6), 1);


       mln_piter_(ima_type) piter (ima.domain());
    int i = 0;
    int nb = 0;
    for_all(piter)
    {
      assert(ima(piter) == i);

      ++nb;
      if (nb == 2)
	i = 2;
      if (nb == 7)
	i = 1;
    }
  }

  /// A third test
  {
    using value::int_u8;

    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8> cmp(lena.domain());

    unsigned n;
    image2d<unsigned> labels =
      labeling::blobs((pw::value(lena) > pw::cst(172u)) | lena.domain(),
		      c4(), n);

    value_enc_image<point2d, int_u8> val_enc =
      value_encode(data::transform(labels, fold_t()));

    data::fill(cmp, literal::zero);
    data::paste(val_enc, cmp);
    std::cout << val_enc.values().size() << std::endl;

    mln_assertion(cmp == data::transform(labels, fold_t()));
    //io::pgm::save(cmp, "output.pgm");
    //io::pgm::save(data::transform(labels, fold_t()), "output2.pgm");
  }
}
