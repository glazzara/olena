// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TOPO_SKELETON_CREST_HH
# define MLN_TOPO_SKELETON_CREST_HH

/// \file mln/topo/skeleton/crest.hh
///
/// Compute skeletization constraints.


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
