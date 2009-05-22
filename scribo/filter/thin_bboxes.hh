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

#ifndef SCRIBO_FILTER_THIN_BBOXES_HH
# define SCRIBO_FILTER_THIN_BBOXES_HH

/// \file scribo/filter/thin_bboxes.hh
///
/// Remove too thin bboxes.

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>
# include <mln/util/array.hh>

# include <scribo/util/text.hh>

namespace scribo
{

  namespace filter
  {

    /// Remove components thinner or equal to \p min_thickness.
    ///
    /// \param[in] input_ a binary image.
    /// \param[in] nbh_ a neighborhood used in labeling algorithms.
    /// \param[in] label_type the label type used for labeling.
    /// \param[in] min_thickness the minimum thickness value.
    ///
    /// \result A binary image without thin components.
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    thin_bboxes(const Image<I>& input_,
		const Neighborhood<N>& nbh_,
		const V& label_type,
		unsigned min_thickness);

    /// Remove lines of text thinner or equal to \p min_thickness.
    ///
    /// \param[in] text lines of text.
    /// \param[in] min_thickness the minimum thickness value.
    ///
    /// \result Lines of text without too thin lines.
    template <typename L>
    inline
    scribo::util::text<L>
    thin_bboxes(const scribo::util::text<L>& text,
		unsigned min_thickness);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      /// Filter Functor.
      /// Return false for all components which are too large.
      template <typename R>
      struct filter_too_thin_component_functor
	: Function_v2b< filter_too_thin_component_functor<R> >
      {

	/// Constructor
	///
	/// \param[in] compbboxes component bounding boxes.
	/// \param[in] min_thickness the minimum thickness allowed.
	filter_too_thin_component_functor(const mln::util::array<R>& compbboxes,
					  unsigned min_thickness)
	  : compbboxes_(compbboxes), min_thickness_(min_thickness)
	{
	}


	/// Return false if the components is thinner than
	/// \p min_thickness_.
	///
	/// \param[in] l An image value.
	bool operator()(const value::label_16& l) const
	{
	  return compbboxes_[l].nrows() > min_thickness_
		&& compbboxes_[l].ncols() > min_thickness_;
	}


	/// Component bounding boxes.
	const mln::util::array<R>& compbboxes_;

	/// The minimum thickness.
	unsigned min_thickness_;
      };


    } //  end of namespace scribo::filter::internal


    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    thin_bboxes(const Image<I>& input_,
		const Neighborhood<N>& nbh_,
		const V& label_type,
		unsigned min_thickness)
    {
      trace::entering("scribo::filter::thin_bboxes");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      V nlabels;
      mln_ch_value(I,V) lbl = labeling::blobs(input, nbh, nlabels);

      typedef accu::bbox<mln_psite(I)> accu_bbox_t;
      typedef mln_result(accu_bbox_t) accu_bbox_res_t;
      typedef mln::util::array<accu_bbox_res_t> compbboxes_t;
      compbboxes_t compbboxes = labeling::compute(accu_bbox_t(), lbl, nlabels);

      typedef internal::filter_too_thin_component_functor<accu_bbox_res_t> func_t;
      func_t fv2b(compbboxes, min_thickness);
      labeling::relabel_inplace(lbl, nlabels, fv2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(lbl) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::thin_bboxes");
      return output;
    }


    template <typename L>
    inline
    scribo::util::text<L>
    thin_bboxes(const scribo::util::text<L>& text,
		unsigned min_thickness)
    {
      trace::entering("scribo::filter::thin_bboxes");

      mln_precondition(text.is_valid());

      typedef mln_site(L) P;
      typedef accu::bbox<P> accu_bbox_t;
      typedef mln_result(accu_bbox_t) accu_bbox_res_t;
      typedef mln::util::array<accu_bbox_res_t> nsitecomp_t;

      typedef internal::filter_too_thin_component_functor<accu_bbox_res_t> func_t;
      func_t is_not_too_thin(text.bboxes(), min_thickness);

      fun::i2v::array<bool> f(text.nbboxes().next(), false);
      f(0) = true;
      mln::util::array<box<P> > bresult;
      bresult.append(box<P>());
      for_all_components(i, text.bboxes())
	if (is_not_too_thin(i))
	{
	  bresult.append(text.bbox(i));
	  f(i) = true;
	}

      util::text<L> output = scribo::make::text(text, f);

      trace::exiting("scribo::filter::thin_bboxes");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_THIN_BBOXES_HH
