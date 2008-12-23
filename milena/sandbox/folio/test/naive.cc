// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file TODO
 *
 * \brief Defines a function that creates a distance map corresponding to a
 * given image.
 */

#ifndef MLN_DT_NAIVE_HH
# define MLN_DT_NAIVE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/literal/zero.hh>
# include <mln/accu/min.hh>

namespace mln
{

  namespace dt
  {

    /*! Calculates the distance map corresponding to a given image
     *
     * \param[in] input_ The binary reference image.
     * \param[in] fun_   The function used for distance aclculus.
     * \return           New distance map image.
     *
     * \pre \p input_ has to be initialized.
     *
     * \fixme Use instead of R the result type of F::operator().
     */
    template <typename I, typename F>
    inline
    mln_ch_value(I, mln_result(F))
    naive(const Image<I>& input_, const Function_v2v<F>& fun_);


# ifndef MLN_INCLUDE_ONLY

    // Facade.

    template <typename I, typename F>
    inline
    mln_ch_value(I, mln_result(F))
    naive(const Image<I>& input_, const Function_v2v<F>& fun_)
    {
      const I& input = exact(input_);
      const F& fun   = exact(fun_);
      mln_precondition(input.has_data());

      mln_ch_value(I, mln_result(F)) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      for_all(p)
      {
	if (input(p))
	  output(p) = literal::zero;
	else
	{
	  // p is in the background so the distance has to be computed.
	  accu::min_<mln_result(F)> min;
          min.init();

          mln_piter(I) q(input.domain());
          for_all(q)
            if (input(q))
              {
		// q is in the object.
//                 metal::vec<2, int> vp = p.to_point(), vq = q.to_point();
//                 min.take(fun_(vp, vq));
		min.take(fun(p - q));
              }
          output(p) = min;
	}
      }

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_NAIVE_HH

#include <iostream>
#include <mln/debug/println.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/fill.hh>
#include <mln/fun/v2v/norm.hh>

int main()
{
  using namespace mln;

  {
    image2d<bool> ima(5,5);
    bool vals[] = { 1, 1, 1, 0, 0,
		    0, 0, 1, 0, 0,
		    0, 0, 1, 0, 0,
		    0, 0, 0, 0, 0,
		    0, 0, 0, 0, 0 };

    data::fill(ima, vals);
    debug::println(ima);

    typedef fun::v2v::l2_norm< algebra::vec<2,float>, float > L2;
    image2d<float> out = dt::naive(ima, L2());

    std::cerr << "Distance:" << std::endl;
    debug::println(out);
  }
}
