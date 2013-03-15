// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_FILTER_INTERNAL_COMPUTE_HH
# define SCRIBO_FILTER_INTERNAL_COMPUTE_HH

/// \file
///
/// Compute filters.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/util/array.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>



namespace scribo
{

  namespace filter
  {

    namespace internal
    {


      using namespace mln;

      /// Apply a filter to an image.
      ///
      /// \param[in] input_ A binary image.
      /// \param[in] nbh_ A neighborhood used in labeling algorithms.
      /// \param[in] label_type The label type used for labeling.
      /// \param[in] filter A filter.
      ///
      /// \result A filtered binary image.
      //
      template <typename I, typename N, typename V, typename F>
      mln_concrete(I)
      compute(const Image<I>& input_,
	      const Neighborhood<N>& nbh_,
	      const V& label_type,
	      const Function_v2b<F>& filter);


      /// Filter an object image.
      ///
      /// \param[in] components A component set.
      /// \param[in] filter A filter functor.
      ///
      /// \result A filtered object image.
      //
      template <typename L, typename F>
      component_set<L>
      compute(const component_set<L>& components,
	      const Function_v2b<F>& filter);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename N, typename V, typename F>
      inline
      mln_concrete(I)
      compute(const Image<I>& input_,
	      const Neighborhood<N>& nbh_,
	      const V& label_type,
	      const Function_v2b<F>& filter)
      {
	mln_trace("scribo::filter::internal::compute");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);

	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());

	V nlabels;
	typedef mln_ch_value(I,V) lbl_t;
	component_set<lbl_t> components
	  = primitive::extract::components(input, nbh, nlabels);

	filter.update_objects(components);
	components.relabel(filter);

	mln_concrete(I) output = duplicate(input);
	data::fill((output | pw::value(components) == literal::zero).rw(), false);

	return output;
      }


      template <typename L, typename F>
      inline
      component_set<L>
      compute(const component_set<L>& components,
	      const Function_v2b<F>& filter)
      {
	mln_trace("scribo::filter::internal::compute");

	mln_precondition(components.is_valid());

	component_set<L> output = components.duplicate();
	output.update_tags(filter, component::Ignored);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::filter::internal

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_INTERNAL_COMPUTE_HH
