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

#ifndef DISTANCE_NAIVE_HH_
# define DISTANCE_NAIVE_HH_

/*! \file TODO
 *
 * \brief Defines a function that creates a distance map corresponding to a
 * given image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/math/sqr.hh>
# include <mln/core/math/sqrt.hh>
# include <mln/core/level/fill.hh>

namespace mln
{
  namespace dist
  {
    /*! Calculates the distance map corresponding to a given image
     *
     * \param[in] img The binary reference image.
     * \param[in] objColor The object's color.
     * \param[in] bgColor The background color.
     * \return New distance map image.
     *
     * \pre \p img has to be initialized.
     *
     * \todo
     */
    template <typename I, typename J>
    Image<J>&
    dt_naive(const Image<I>& img,
	     const mln_value(I)& objColor,
	     const mln_value(I)& bgColor);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename J>
      inline void
      dt_naive_calc(mln_value(J)& res,
		    const mln_point(I)& ref,
		    const mln_point(I)& test)
      {
	mln_value(J) sum = 0;

	for (unsigned i = 0; i < I::pset::dim; ++i)
	  sum += math::sqr(ref[i] - test[i]);

	res = math::sqrt(sum);
      }
    }

    // Facade.
    template <typename I, typename J>
    inline Image<J>&
    dt_naive(const Image<I>& img,
	     const mln_value(I)& objColor,
	     const mln_value(I)& bgColor)
    {
      Image<J> res(img.domain());
      mln_piter(I) p(img.domain());
      mln_piter(I) q(img.domain());

      level::fill(res, literal::zero);

      for_all(p)
      {
	if (img(p) == bgColor)
	  continue;

	for_all(q)
	{
	  if (img(q) == objColor)
	    continue;

	  dt_naive_calc(res(q), p, q));
	}
      }

      return res;
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::dist
} // end of namespace mln

#endif /* !DISTANCE_NAIVE_HH_ */
