// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_SKELETON_CREST_HH
# define MLN_TOPO_SKELETON_CREST_HH

/// \file
///
/// \brief Compute skeletization constraints.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>


namespace mln
{

  namespace topo
  {

    namespace skeleton
    {


      /// Compute skeletization constraints.
      ///
      /// \param[in] input_	A binary image.
      /// \param[in] dist_map_	A distance map of \p input. Contains the
      ///			inner object distance map.
      /// \param[in] nbh_	A neighborhood.
      ///
      /// \result A binary image.
      //
      /*!
       *
       *  This implementation is based on the following article:
       *    K. W. Kang, J. W. Suh, and J. H. Kim. Skeletonization of grayscale
       *    character images using pixel superiority index. In Proc. 3rd IAPR
       *    Workshop on Document Analysis Systems, pages 326-335, Nagano,
       *    Japan, 1998.
       *
       *  Abstract:
       *    "In this paper, we present pixel superiority index as a tool for
       *    designing a skeletonization algorithm which utilizes topographic
       *    features efficiently. We clarify a relationship between pixel
       *    superiority index and topographic features. Then, using the
       *    relationship, we transform a problem of skeletonization into a
       *    problem of skeleton growing. [...]"
       *
       *
       *  In Milena, the Pixel Superiority index is defined as follow:
       *  Let v = p.val(), the Pixel superiority index of p is the number of
       *  neighbor pixels having their value/level inferior or equal to
       *  p.val().
       *
       *  This algorithm keeps sites having their pixel superiority index
       *  greater than 5.
       *
       *  For good results with 2D images, we advice you to use c8() as
       *  neighborhood.
       *
       */
      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input_, const Image<D>& dist_map_,
	    const Neighborhood<N>& nbh_);



# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename D, typename N>
      mln_concrete(I)
      crest(const Image<I>& input_, const Image<D>& dist_map_,
	    const Neighborhood<N>& nbh_)
      {
	trace::entering("topo::skeleton::crest");
	const I& input = exact(input_);
	const D& dist_map = exact(dist_map_);
	const N& nbh = exact(nbh_);

	mlc_equal(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());
	mln_precondition(dist_map.is_valid());
	mln_precondition(nbh.is_valid());

	mln_concrete(I) is_crest;
	initialize(is_crest, input);
	data::fill(is_crest, false);

	mln_piter(I) p(input.domain());
	mln_niter(N) n(nbh, p);
	for_all(p)
	{
	  if (!input(p) || dist_map(p) < 0)
	    continue;

	  unsigned nb_eq = 0;
	  unsigned nb_gt = 0;
	  unsigned nb_lt = 0;
	  for_all(n)
	    if (input.domain().has(n))
	    {
	      if (dist_map(n) == dist_map(p))
		++nb_eq;
	      else if (dist_map(n) > dist_map(p))
		++nb_gt;
	      else
		++nb_lt;
	    }

	  if ((nb_lt + nb_eq) > 5) // Pixel Superiority index
	    is_crest(p) = true;
	}

	trace::exiting("topo::skeleton::crest");
	return is_crest;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::topo::skeleton

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_SKELETON_CREST_HH
