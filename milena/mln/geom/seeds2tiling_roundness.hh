// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH
# define MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH

/// \file
///
/// seeds2tiling with a method which makes tiles more roundness.

# include <map>

# include <mln/core/concept/image.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/core/site_set/p_priority.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/estim/min_max.hh>
# include <mln/algebra/vec.hh>
# include <mln/geom/chamfer.hh>


namespace mln
{
  namespace geom
  {

    /*! \brief Take a labeled image \p ima_ with seeds and extend them
      until creating tiles rounder than the primary version.

      \param[in,out] ima_   The labeled image with seed.
      \param[in]     w_win  The weight window using by geom::chamfer to
                            compute distance.
      \param[in]     max    Unsigned using by geom::chamfer to compute
                            the distance.
      \param[in]     nbh_   The neighborhood to use on this algorithm.

      \pre \p ima_ has to be initialized.

      \ingroup mlngeom
     */
    template <typename I, typename N>
    I
    seeds2tiling_roundness (Image<I>& ima_, const w_window2d_int& w_win,
			    unsigned max, const Neighborhood<N>& nbh_);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename N>
      inline
      I
      seeds2tiling_roundness(Image<I>& ima_, const w_window2d_int& w_win,
			     unsigned max, const Neighborhood<N>& nbh_)
      {
	trace::entering("geom::impl::seed2tiling_roundness");

	I& ima = exact(ima_);
	const N& nbh = exact(nbh_);
	image2d<unsigned> dist = geom::chamfer(ima, w_win, max);

	I out = duplicate(ima_);
	p_priority<unsigned, p_queue_fast<mln_psite(I)> > q;

	// Init.
	{
	  mln_piter(I) p(ima.domain());

	  for_all(p)
	      q.push(max - dist(p), p);
	}


	// Body: alternative version.
	{
	  while (! q.is_empty())
	    {
	      mln_psite(I) p = q.pop_front();

	      if (out(p) != literal::zero) // p has already been processed so ignore
		continue;
	      mln_niter(N) n(nbh, p);

	      for_all(n) if (ima.has(n))
		if (out(n) != literal::zero)
		  out(p) = out(n);
	    }
	}

	trace::exiting("geom::impl::seed2tiling_roundness");
	return out;
      }

    } // end of namespace mln::geom::impl


    // Facade
    template <typename I, typename N>
    inline
    I
    seeds2tiling_roundness(Image<I>& ima_, const w_window2d_int& w_win,
			   unsigned max, const Neighborhood<N>& nbh)
    {
      trace::entering("geom::seed2tiling_roundness");

      mln_precondition(exact(ima_).is_valid());
      I output = impl::seeds2tiling_roundness(ima_, w_win, max, nbh);

      trace::exiting("geom::seed2tiling_roundness");
      return output;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_SEEDS2TILING_ROUNDNESS_HH
