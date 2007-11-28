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

#ifndef MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH
# define MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH

/*! \file mln/geom/seeds2tiling_roundness.hh
 *
 * \brief seeds2tiling with a method which makes tiles more roundness.
 */

# include <map>


# include <mln/core/p_priority_queue_fast.hh>
# include <mln/core/clone.hh>
# include <mln/accu/mean.hh>
# include <mln/estim/min_max.hh>
# include <mln/metal/vec.hh>
# include <mln/geom/chamfer.hh>


namespace mln
{
  namespace geom
  {


    /*! Take a labeled image \p ima_ with seeds and extend them until
     *  creating tiles nore roundness that the primary version.
     *
     * \param[in,out] ima The labeled image with seed.
     * \param[in] win_w The weight window using by geom::chamfer to compute distance.
     * \param[in] max Unsigned using by geom::chamfer to compute distance.
     * \param[in] nbh The neighborhood to use on this algorithm.
     *
     * \pre \p ima has to be initialized.
     *
     */
    template <typename I, typename N>
    I
    seeds2tiling_roundness (Image<I>& ima_, const w_window2d_int& w_win, unsigned max,
			    const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename N>
      inline
      I
      seeds2tiling_roundness(Image<I>& ima_, const w_window2d_int& w_win, unsigned max,
			     const Neighborhood<N>& nbh)
      {
	trace::entering("geom::impl::seed2tiling_roundness");

	I& ima = exact(ima_);
	image2d<unsigned> dist = geom::chamfer(ima, w_win, max);
	I out = clone(ima_);
	p_priority_queue_fast<mln_psite(I), unsigned> q;

	// Init.
	{
	  mln_piter(I) p(ima.domain());

	  for_all(p)
	    q.push_force(p, max - dist(p));
	}


	// Body: alternative version.
	{
	  while (! q.is_empty())
	    {
	      mln_psite(I) p = q.front();
	      q.pop();
	      if (out(p) != 0) // p has already been processed so ignore
		continue;
	      mln_niter(N) n(nbh, p);

	      for_all(n) if (ima.has(n))
		if (out(n) != 0)
		  out(p) = out(n);
	    }
	}

	trace::exiting("geom::impl::seed2tiling_roundness");
	return out;
      }

    } // end of namespace mln::geom::impl


    /// Facade
    template <typename I, typename N>
    inline
    I
    seeds2tiling_roundness(Image<I>& ima_, const w_window2d_int& w_win, unsigned max,
			   const Neighborhood<N>& nbh)
    {
      trace::entering("geom::seed2tiling_roundness");

      mln_precondition(exact(ima_).has_data());
      I output = impl::seeds2tiling_roundness(ima_, w_win, max, nbh);

      trace::exiting("geom::seed2tiling_roundness");
      return output;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH
