// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FILTER_LARGE_COMPONENTS_HH
# define SCRIBO_FILTER_LARGE_COMPONENTS_HH

/// \file scribo/filter/large_components.hh
///
/// Remove large components in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/relabel.hh>

# include <mln/util/array.hh>
# include <mln/value/label_16.hh>

# include <mln/pw/all.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// Remove large components in a binary image.
    /// Set to 'false' all the removed components.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] nbh_	      A neighborhood used for labeling \p input_.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_size   The minimum cardinality of a component.
    ///
    /// \return A binary image without large components.
    template <typename I, typename N, typename V>
    mln_concrete(I)
    large_components(const Image<I>& input_,
			       const Neighborhood<N>& nbh_,
			       const V& label_type,
			       unsigned max_size);


    /// Remove large bboxes in a binary image.
    /// Set to 'false' all the removed bboxes.
    ///
    /// \param[in] input_     A binary image.
    /// \param[in] bboxes     Bounding boxes of components extracted from \p
    ///			      input_.
    /// \param[in] max_size   The minimum cardinality of a component.
    ///
    /// \return A binary image without large bboxes.
    template <typename P>
    util::array< box<P> >
    large_components(const util::array< box<P> >& bboxes,
		     unsigned max_size);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// Filter Functor. Return false for all components which are too
      /// large.
      template <typename R>
      struct filter_large_components_functor
	: Function_l2b< filter_large_and_large_functor<R> >
      {
	filter_large_components_functor(const util::array<R>& nsitecomp,
					unsigned max_size)
	  : nsitecomp_(nsitecomp), max_size_(max_size)
	{
	}


	/// Return false if the components area is strictly inferior to
	/// \p max_size_.
	bool operator()(const value::label_16& l) const
	{
	  return nsitecomp_[l] <= max_size_;
	}


	const util::array<R>& nsitecomp_;
	unsigned max_size_;
      };


    } //  end of namespace scribo::filter::internal



    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    large_components(const Image<I>& input_,
		     const Neighborhood<N>& nbh_,
		     const V& label_type,
		     unsigned max_size)
    {
      trace::entering("scribo::filter::large_components");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      V nlabels;
      mln_ch_value(I,V) lbl = labeling::blobs(input, nbh, nlabels);

      typedef accu::count<mln_psite(I)> accu_count_t;
      typedef mln_result(accu_count_t) accu_count_res_t;
      typedef util::array<accu_count_res_t> nsitecomp_t;
      nsitecomp_t nsitecomp = labeling::compute(accu_count_t(), lbl, nlabels);

      typedef internal::filter_large_and_large_functor<accu_count_res_t> func_t;
      func_t fl2b(nsitecomp, max_size);
      labeling::relabel_inplace(lbl, nlabels, fl2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(lbl) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::large_components");
      return output;
    }


    template <typename P>
    inline
    util::array< box<P> >
    large_components(const util::array< box<P> >& bboxes,
		     unsigned max_size)
    {
      trace::entering("scribo::filter::large_components");

      mln_precondition(input.is_valid());

      typedef accu::count<P> accu_count_t;
      typedef mln_result(accu_count_t) accu_count_res_t;
      typedef util::array<accu_count_res_t> nsitecomp_t;

      util::array<box<P> > result;
      result.append(box<P>());
      for_all_components(i, bboxes)
      {
	accu_count_res_t count = set::compute(accu_count_t(), bboxes[i]);
	if (count <= max_size)
	  result.append(bboxes[i]);
      }

      trace::exiting("scribo::filter::large_components");
      return result;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_LARGE_COMPONENTS_HH
