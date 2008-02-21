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

#include <iostream>

#include <mln/core/image2d.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/accu/min.hh>

#ifndef CHAMFER_DT_HH
# define CHAMFER_DT_HH

namespace mln
{
  template<typename I>
  inline
  mln_ch_value(I, float)
  chamfer_dt(const Image<I>& input_)
  {
    const I& input = exact(input_);
    mln_precondition(input.has_data());

    mln_ch_value(I, float) output;
    initialize(output, input);

    accu::min_<int> min;
    w_window2d_int chamferTop = make::mk_chamfer_3x3_int<3, 4>();

    // initialization
    {
      mln_fwd_piter(I) p(input.domain());
      for_all(p)
	output(p) = input(p) ? 0 : mln_max(int);
    }

    // First pass.
    {
      mln_fwd_piter(I) p(input.domain());
      mln_qiter(w_window2d_int) q(chamferTop, p);
      for_all(p)
      {
	min.init();
	for_all(q)
	  if (input.has(q) && output(q) != mln_max(int))
	    min.take(output(q) + q.w());
	if (output(p) != mln_max(int))
	  min.take(output(p));
	output(p) = min.to_result();
      }
    }

    // Second pass.
    {
      w_window2d_int chamferBottom = chamferTop.sym();
      mln_bkd_piter(I) p(input.domain());
      mln_qiter(w_window2d_int) q(chamferBottom, p);
      for_all(p)
      {
	min.init();
	for_all(q)
	  if (input.has(q) && output(q) != mln_max(int))
	    min.take(output(q) + q.w());
	if (output(p) != mln_max(int))
	  min.take(output(p));
	output(p) = min.to_result();
      }
    }

    return output;
  }
}

#endif /* ! CHAMFER_DT_HH */

int main()
{
  using namespace mln;

  image2d<bool> ima(5,5);
  bool vals[] = { 1, 1, 1, 0, 0,
                  0, 0, 1, 0, 0,
                  0, 0, 1, 0, 0,
                  0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0 };

  level::fill(ima, vals);

  debug::println(ima);
  debug::println(chamfer_dt(ima));
}
