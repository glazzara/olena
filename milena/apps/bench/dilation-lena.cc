// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>

#include <mln/accu/stat/max.hh>

#include <mln/util/timer.hh>

#include "apps/bench/static_window.hh"
#include "apps/bench/static_dpoints_pixter.hh"
#include "apps/data.hh"


namespace nongen
{
  typedef mln::image2d<mln::value::int_u8> image;

  image dilation(const image& input)
  {
    image output (input.nrows(), input.ncols());  // Initialize an output image.
    for (unsigned int r = 0; r < input.nrows(); ++r)  // Iterate on rows.
      for (unsigned int c = 0; c < input.ncols(); ++c)
	{  // Iterate on columns.
	  unsigned char sup = input.at_(r, c);
	  if (r != 0                   && input.at_(r-1, c) > sup)   sup = input.at_(r-1, c);
	  if (r != input.nrows() - 1   && input.at_(r+1, c)  > sup)  sup = input.at_(r+1, c);
	  if (c != 0                   && input.at_(r, c-1) > sup)   sup = input.at_(r, c-1);
	  if (c != input.ncols() - 1   && input.at_(r, c+1) > sup)   sup = input.at_(r, c+1);
	  output.at_(r, c) = sup;
	}
    return output;
  }
}

namespace gen
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    mln_concrete(I) output; initialize(output, input);  // Initialize output.
    mln_piter(I) p(input.domain());  // Iterator on sites of the domain of `input'.
    mln_qiter(W) q(win, p);  // Iterator on the neighbors of `p' w.r.t. `win'.
    for_all(p)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln::accu::stat::max<mln_value(I)> sup;  // Accumulator computing the supremum.
      for_all(q) if (input.has(q))
	sup.take(input(q));
      output(p) = sup.to_result();
    }
    return output;
  }
}

namespace fast
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    typedef mln_concrete(I) O;
    O output; initialize(output, input);  // Initialize output.

    mln_pixter(const I) pi(input);  // Iterator on the pixels of `input'.
    mln_pixter(O) po(output);  // Iterator on the pixels of `output'.

    mln_qixter(const I, W) q(pi, win);  // Iterator on the neighbors of `p' w.r.t. `win'.
    for_all_2(pi, po)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln::accu::stat::max<mln_value(I)> sup;  // Accumulator computing the supremum.
      for_all(q)
	sup.take(q.val());
      po.val() = sup.to_result();
    }
    return output;
  }
}

namespace fast_noaccu
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    typedef mln_concrete(I) O;
    O output; initialize(output, input);  // Initialize output.

    mln_pixter(const I) pi(input);  // Iterator on the pixels of `input'.
    mln_pixter(O) po(output);  // Iterator on the pixels of `output'.

    mln_qixter(const I, W) q(pi, win);  // Iterator on the neighbors of `p' w.r.t. `win'.
    for_all_2(pi, po)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln_value(I) sup = mln_min(mln_value(I));
      for_all(q)
	if (q.val() > sup)
	  sup = q.val();
      po.val() = sup;
    }
    return output;
  }
}

namespace faster
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    typedef mln_concrete(I) O;
    O output; initialize(output, input);  // Initialize output.

    mln_pixter(const I) p(input);  // Iterator on the pixels of `input'.

    mln_qixter(const I, W) q(p, win);  // Iterator on the neighbors of `p' w.r.t. `win'.
    for_all(p)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln::accu::stat::max<mln_value(I)> sup;  // Accumulator computing the supremum.
      for_all(q)
	sup.take(q.val());
      *(output.buffer() + p.offset()) = sup.to_result();
    }
    return output;
  }
}

namespace faster_noaccu
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    typedef mln_concrete(I) O;
    O output; initialize(output, input);  // Initialize output.

    mln_pixter(const I) p(input);  // Iterator on the pixels of `input'.

    mln_qixter(const I, W) q(p, win);  // Iterator on the neighbors of `p' w.r.t. `win'.
    for_all(p)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln_value(I) sup = mln_min(mln_value(I));
      for_all(q)
	if (q.val() > sup)
	  sup = q.val();
      *(output.buffer() + p.offset()) = sup;
    }
    return output;
  }
}

namespace fast_static
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    typedef mln_concrete(I) O;
    O output; initialize(output, input);  // Initialize output.

    mln_pixter(const I) pi(input);  // Iterator on the pixels of `input'.
    mln_pixter(O) po(output);  // Iterator on the pixels of `output'.

    typedef mln::static_dpoints_fwd_pixter<const I, W::Size> mln_static_qixter;
    mln_static_qixter q(pi, win);
    for_all_2(pi, po)
    {
      // FIXME: Cheat: replace the accu::supremum by a maximum.
      mln::accu::stat::max<mln_value(I)> sup;  // Accumulator computing the supremum.
      for_all(q)
	sup.take(q.val());
      po.val() = sup.to_result();
    }
    return output;
  }
}


int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 1;
  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_APPS_DIR "/bench/lena1024.pgm");

  image2d<int_u8> d;
  util::timer t;

  t.start();
  d = nongen::dilation(lena);
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-spe.pgm");

  t.start();
  d = gen::dilation(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-gen.pgm");

  t.start();
  d = fast::dilation(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-fast.pgm");

  t.start();
  d = fast_noaccu::dilation(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-fast_noaccu.pgm");

  t.start();
  d = faster::dilation(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-faster.pgm");

  t.start();
  d = faster_noaccu::dilation(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-faster_noaccu.pgm");

  const unsigned n = 5;
  mln::dpoint2d dps[n] = { mln::dpoint2d( 0, -1),
			   mln::dpoint2d(-1,  0),
			   mln::dpoint2d( 0,  0),
			   mln::dpoint2d(+1,  0),
			   mln::dpoint2d( 0, +1) };
  mln::util::static_array<mln::dpoint2d, n> sa(dps, dps + n);
  mln::static_window<mln::dpoint2d, n> static_win_c4p (sa);

  t.start();
  d = fast_static::dilation(lena, static_win_c4p);
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(d, "dilation-lena-out-fast_static.pgm");
}
