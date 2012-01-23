// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_GEOM_SEEDS2TILING_HH
# define MLN_GEOM_SEEDS2TILING_HH

/// \file
///
/// Convert a binary image with seeds into a labeled image.

# include <map>

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/p_queue.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/estim/min_max.hh>
# include <mln/algebra/vec.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Take a labeled image \p ima_ with seeds and extend them
      until creating tiles.

      \param[in,out] ima_ The labeled image with seed.
      \param[in] nbh The neighborhood to use on this algorithm.

      \return A tiled image.

      \pre \p ima_ has to be initialized.

      \ingroup mlngeom
    */
    template <typename I, typename N>
    mln_concrete(I) seeds2tiling (const Image<I>& ima_,
				  const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      /// Generic implementation of geom::seed2tiling.
      ///
      /// \param[in,out] ima_ The labeled image with seed.
      /// \param[in] nbh_ The neighborhood to use on this algorithm.
      //
      template <typename I, typename N>
      inline
      mln_concrete(I)
      seeds2tiling (const Image<I>& ima_,
		    const Neighborhood<N>& nbh_)
      {
	trace::entering("geom::impl::seed2tiling");

	mln_precondition(exact(ima_).is_valid());
	mln_precondition(exact(nbh_).is_valid());

	const I& ima = exact(ima_);
	mln_concrete(I) out = duplicate(ima_);
	const N& nbh = exact(nbh_);
	p_queue<mln_psite(I)> q;

	// Init.
	{
	  mln_piter(I) p(ima.domain());
	  mln_niter(N) n(nbh, p);

	  for_all(p) if (ima(p) == 0)
	    for_all(n) if (ima(n) != 0)
	      {
		q.push(p);
		break;
	      }
	}

	// Body.
	{
	  while (! q.is_empty())
	    {
	      mln_psite(I) p = q.front();
	      q.pop();
	      if (out(p) != 0) // p has already been processed so ignore
		continue;

	      mln_niter(N) n(nbh, p);
	      for_all(n) if (ima.has(n))
		{
		  if (out(n) != 0)
		    out(p) = out(n);
		  else
		    q.push(n); // n may already be in the queue,
			       // yet we then queue again this psite
		}
	    }
	}

	trace::exiting("geom::impl::seed2tiling");
	return out;
      }

    } // end of namespace mln::geom::impl



    // Facade

    template <typename I, typename N>
    inline
    mln_concrete(I) seeds2tiling(const Image<I>& ima_, const Neighborhood<N>& nbh)
    {
      trace::entering("geom::seed2tiling");

      mln_precondition(exact(ima_).is_valid());
      mln_precondition(exact(nbh).is_valid());

      mln_concrete(I) output = impl::seeds2tiling(ima_, nbh);

      trace::exiting("geom::seed2tiling");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_SEEDS2TILING_HH
