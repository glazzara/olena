// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_BENCH_DILATION_LENA_HH
#define  APPS_BENCH_DILATION_LENA_HH

/// \file
/// \brief Dilation benchmark cases.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>

#include <mln/accu/stat/max.hh>

#include <mln/util/timer.hh>

#include "apps/bench/static_window.hh"
#include "apps/bench/static_dpoints_pixter.hh"
#include "apps/bench/trait.hh"


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

namespace nongen_2ptr
{
  typedef mln::image2d<mln::value::int_u8> image;

  image dilation(const image& input)
  {
    typedef mln::value::int_u8 val_t;
    // Offsets corresponding to a 4-c window on INPUT.
    ptrdiff_t win_offset[4] = { &input.at_(-1,  0) - &input.at_(0, 0),
				&input.at_(+1,  0) - &input.at_(0, 0),
				&input.at_( 0, -1) - &input.at_(0, 0),
				&input.at_( 0, +1) - &input.at_(0, 0) };

    image output (input.nrows(), input.ncols());  // Initialize an output image.
    for (unsigned int r = 0; r < input.nrows(); ++r)  // Iterate on rows.
      {
	const val_t* pi = &input.at_(r, 0);
	val_t* po = &output.at_(r, 0);
	for (; pi < &input.at_(r, 0) + input.ncols(); ++pi, ++po)
	{
	  unsigned char sup = *pi;
	  // (-1, 0) neighbor.
	  if (r  != 0
	      && *(pi + win_offset[0]) > sup)
	    sup = *(pi + win_offset[0]);
	  // (+1, 0) neighbor.
	  if (r  != input.nrows() - 1
	      && *(pi + win_offset[1]) > sup)
	    sup = *(pi + win_offset[1]);
	  // (0, -1) neighbor.
	  if (pi != &input.at_(r, 0)
	      && *(pi + win_offset[2]) > sup)
	    sup = *(pi + win_offset[2]);
	  // (0, +1) neighbor.
	  if (pi  != &input.at_(r, 0) + input.ncols() - 1
	      && *(pi + win_offset[3]) > sup)
	    sup = *(pi + win_offset[3]);
	  *po = sup;
	}
      }
    return output;
  }
}

namespace nongen_1ptr
{
  typedef mln::image2d<mln::value::int_u8> image;

  image dilation(const image& input)
  {
    typedef mln::value::int_u8 val_t;
    // Offsets corresponding to a 4-c window on INPUT.
    ptrdiff_t win_offset[4] = { &input.at_(-1,  0) - &input.at_(0, 0),
				&input.at_(+1,  0) - &input.at_(0, 0),
				&input.at_( 0, -1) - &input.at_(0, 0),
				&input.at_( 0, +1) - &input.at_(0, 0) };

    image output;
    initialize(output, input);
    // Offset between a the pixel located at the same position in
    // INPUT and OUTPUT.
    ptrdiff_t output_offset = &output.at_(0,  0) - &input.at_(0, 0);

    for (unsigned int r = 0; r < input.nrows(); ++r)  // Iterate on rows.
      {
	for (const val_t* pi = &input.at_(r, 0);
	     pi < &input.at_(r, 0) + input.ncols();
	     ++pi)
	{
	  unsigned char sup = *pi;
	  // (-1, 0) neighbor.
	  if (r  != 0
	      && *(pi + win_offset[0]) > sup)
	    sup = *(pi + win_offset[0]);
	  // (+1, 0) neighbor.
	  if (r  != input.nrows() - 1
	      && *(pi + win_offset[1]) > sup)
	    sup = *(pi + win_offset[1]);
	  // (0, -1) neighbor.
	  if (pi != &input.at_(r, 0)
	      && *(pi + win_offset[2]) > sup)
	    sup = *(pi + win_offset[2]);
	  // (0, +1) neighbor.
	  if (pi  != &input.at_(r, 0) + input.ncols() - 1
	      && *(pi + win_offset[3]) > sup)
	    sup = *(pi + win_offset[3]);
	  const_cast<val_t&>(*(pi + output_offset)) = sup;
	}
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
    // `fast_static' has the same implementation as `fast'.
    return ::fast::dilation(input, win);
  }
}

namespace faster_static
{
  using namespace mln;

  template <typename I, typename W>
  mln_concrete(I) dilation(const I& input, const W& win)
  {
    // `faster_static' has the same implementation as `faster'.
    return ::faster::dilation(input, win);
  }
}

#endif // ! APPS_BENCH_DILATION_LENA_HH
