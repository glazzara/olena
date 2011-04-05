// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_ESTIM_INTERNAL_COMPUTE_SKELETON_HH
# define SCRIBO_ESTIM_INTERNAL_COMPUTE_SKELETON_HH

/// \file
///
/// \brief Compute a skeleton useful for font estimations.


# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/topo/skeleton/crest.hh>
# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/transform/distance_front.hh>
# include <mln/logical/not.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/value/int_u8.hh>
# include <mln/arith/revert.hh>
# include <mln/morpho/skeleton_constrained.hh>
# include <mln/core/routine/extend.hh>
# include <mln/labeling/blobs.hh>
# include <mln/accu/stat/mean.hh>
# include <mln/value/rgb8.hh>


namespace scribo
{

  namespace estim
  {

    namespace internal
    {
      using namespace mln;


      /// \brief Compute a skeleton useful for font estimations.
      //
      template <typename I>
      mln_concrete(I)
      compute_skeleton(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      mln_concrete(I)
      compute_skeleton(const Image<I>& input_)
      {
	trace::entering("scribo::estim::internal::compute_skeleton");

	const I& input = exact(input_);
	mlc_is(mln_value(I), bool)::check();
	mln_precondition(input.is_valid());

	int psi = 7;
	int     vals[] = { 0, 9, 0, 9, 0,
			   9, 6, 4, 6, 9,
			   0, 4, 0, 4, 0,
			   9, 6, 4, 6, 9,
			   0, 9, 0, 9, 0 };

	mln_ch_value(I,value::int_u8)
	  dist = transform::distance_front(logical::not_(input), c8(),
					   mln::make::w_window2d_int(vals),
					   mln_max(value::int_u8));

	mln_ch_value(I,value::int_u8) dist_map = arith::revert(dist);
	mln_concrete(I) K = topo::skeleton::crest(input, dist, c8(), psi);

	mln_concrete(I) skel =
	  morpho::skeleton_constrained(input, c8(),
				       topo::skeleton::is_simple_point<I,neighb2d>,
				       extend(K, false), dist_map);

	trace::exiting("scribo::estim::internal::compute_skeleton");
	return skel;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::estim::internal

  } // end of namespace scribo::estim

} // end of namespace scribo

#endif // ! SCRIBO_ESTIM_INTERNAL_COMPUTE_SKELETON_HH
