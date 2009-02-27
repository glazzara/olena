// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/util/timer.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/morpho/leveling_filter.hh>
#include <mln/morpho/algebraic_filter.hh>

#include "tests/data.hh"

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc != 2)
    {
      std::cout << "Usage: ./connected_filter lambda" << std::endl;
      return 0;
    }

  typedef mln::image2d<int_u8> I;
  I lena;

  float elapsed;
  mln::util::timer chrono;
  mln::morpho::attribute::volume<I> c;
  mln::morpho::attribute::card<I> c2;
  int lambda = atoi(argv[1]);

  mln::io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
  I out;

  std::cout << "Test with card attribute:" << std::endl;
  chrono.start();
  out = mln::canvas::morpho::attribute_filter(lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Full auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_auto.pgm");

  chrono.start();
  out = mln::morpho::algebraic_filter(lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Algebraic Auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_algebraic.pgm");

  chrono.start();
  out = mln::canvas::morpho::internal::attribute_filter_dispatch(metal::false_(), lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Force Slow) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_slow.pgm");

  // Try force algebraic dispatch with wrong accu (volume).
  // mln::morpho::algebraic_filter(lena, c4(), c, lambda, true);

  std::cout << "Test with volume attribute:" << std::endl;
  chrono.start();
  out = mln::canvas::morpho::attribute_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Full auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_auto.pgm");

  chrono.start();
  out = mln::morpho::leveling_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Leveling Auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_leveling.pgm");

  chrono.start();
  out = mln::canvas::morpho::internal::attribute_filter_dispatch(metal::false_(), lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(Force Slow) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_slow.pgm");

  // Try force leveling dispatch with wrong accu (card).
  // mln::morpho::leveling_filter(lena, c4(), c2, lambda, true);


}
