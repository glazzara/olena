// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef COLOR_INTERNAL_GRADIENT_HH_
# define COLOR_INTERNAL_GRADIENT_HH_

#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/math/diff_abs.hh>

#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>
#include <mln/accu/image/to_result.hh>

#include <mln/accu/max.hh>

namespace mln
{

  value::int_u8
  dist(const value::rgb8& c1,
       const value::rgb8& c2)
  {
    unsigned d = 0;
    d += math::diff_abs(c1.red(), c2.red());
    d += math::diff_abs(c1.green(), c2.green());
    d += math::diff_abs(c1.blue(), c2.blue());
    if (d > 255)
      d = 255;
    return d;
  }

  template <typename N>
  image2d<value::int_u8>
  color_gradient(const image2d<value::rgb8>& input,
		 const N& nbh)
  {
    using value::int_u8;
    image2d<int_u8> output(input.domain());

    mln_piter(box2d) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      int_u8 d = 0u;
      for_all(n) if (input.domain().has(n))
	{
	  int_u8 d_ = dist(input(p), input(n));
	  if (d_ > d)
	    d = d_;
	}
      output(p) = d;
    }

    return output;
  }



  template <typename I>
  image2d<value::int_u8>
  get_red(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().red();
    return output;
  }

  template <typename I>
  image2d<value::int_u8>
  get_green(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().green();
    return output;
  }

  template <typename I>
  image2d<value::int_u8>
  get_blue(const I& input)
  {
    typedef image2d<value::int_u8> J;
    J output(input.domain());

    mln_pixter(const I) p_in(input);
    mln_pixter(J) p_out(output);
    for_all_2(p_in, p_out)
      p_out.val() = p_in.val().blue();
    return output;
  }

  image2d<value::int_u8>
  color_internal_gradient(const image2d<value::rgb8>& input)
  {
    typedef accu::max< value::int_u8 > A;
    image2d<A> grad(input.domain());

    using morpho::elementary::gradient_internal;

    accu::image::init(grad);

    accu::image::take(grad,
		      gradient_internal(get_red(input), c4()));
    accu::image::take(grad,
		      gradient_internal(get_green(input), c4()));
    accu::image::take(grad,
		      gradient_internal(get_blue(input), c4()));

    return accu::image::to_result(grad);
  }

} // mln

#endif /* !COLOR_INTERNAL_GRADIENT_HH_ */
