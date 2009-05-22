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

#ifndef SCRIBO_FILTER_THICK_BBOXES_HH
# define SCRIBO_FILTER_THICK_BBOXES_HH

/// \file scribo/filter/thick_bboxes.hh
///
/// Remove too thick bboxes.

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>
# include <mln/util/array.hh>

# include <scribo/util/text.hh>

# include <scribo/make/text.hh>


namespace scribo
{

  namespace filter
  {

    /// Remove components thicker or equal to \p max_thickness.
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] nbh_ A neighborhood used in labeling algorithms.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result A binary image without thick components.
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    thick_bboxes(const Image<I>& input_,
		 const Neighborhood<N>& nbh_,
		 const V& label_type,
		 unsigned max_thickness);


    /// Remove lines of text thicker or equal to \p max_thickness.
    ///
    /// \param[in] text Lines of text.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result Lines of text without too thick lines.
    template <typename L>
    inline
    scribo::util::text<L>
    thick_bboxes(const scribo::util::text<L>& text,
		 unsigned max_thickness);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      /// Filter Functor. Return false for all components which are too
      /// large.
      template <typename R>
      struct filter_too_thick_component_functor
	: Function_l2b< filter_too_thick_component_functor<R> >
      {

	/// Constructor
	///
	/// \param[in] compbboxes component bounding boxes.
	/// \param[in] max_thickness the maximum thickness allowed.
	filter_too_thick_component_functor(const mln::util::array<R>& compbboxes,
					  unsigned max_thickness)
	  : compbboxes_(compbboxes), max_thickness_(max_thickness)
	{
	}


	/// Return false if the components is thicker than
	/// \p max_thickness_.
	///
	/// \param[in] l An image value.
	bool operator()(const value::label_16& l) const
	{
	  return compbboxes_[l].nrows() < max_thickness_
		&& compbboxes_[l].ncols() < max_thickness_;
	}


	/// Component bounding boxes.
	const mln::util::array<R>& compbboxes_;

	/// The maximum thickness.
	unsigned max_thickness_;
      };


    } //  end of namespace scribo::filter::internal


    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    thick_bboxes(const Image<I>& input_,
		const Neighborhood<N>& nbh_,
		const V& label_type,
		unsigned max_thickness)
    {
      trace::entering("scribo::filter::thick_bboxes");

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

      typedef internal::filter_too_thick_component_functor<accu_bbox_res_t> func_t;
      func_t fl2b(compbboxes, max_thickness);
      labeling::relabel_inplace(lbl, nlabels, fl2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(lbl) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::thick_bboxes");
      return output;
    }


    template <typename L>
    inline
    scribo::util::text<L>
    thick_bboxes(const scribo::util::text<L>& text,
		 unsigned max_thickness)
    {
      trace::entering("scribo::filter::thick_bboxes");

      mln_precondition(text.is_valid());

      typedef mln_site(L) P;
      typedef accu::bbox<P> accu_bbox_t;
      typedef mln_result(accu_bbox_t) accu_bbox_res_t;
      typedef mln::util::array<accu_bbox_res_t> nsitecomp_t;

      typedef internal::filter_too_thick_component_functor<accu_bbox_res_t> func_t;
      func_t is_not_too_thick(text.bboxes(), max_thickness);

      fun::i2v::array<bool> f(text.nbboxes().next(), false);
      f(0) = true;
      mln::util::array<box<P> > bresult;
      bresult.append(box<P>());
      for_all_components(i, text.bboxes())
	if (is_not_too_thick(i))
	{
	  bresult.append(text.bbox(i));
	  f(i) = true;
	}

      util::text<L> output = scribo::make::text(text, f);

      trace::exiting("scribo::filter::thick_bboxes");
      /// FIXME: construct a new util::text from the old one.
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_THICK_BBOXES_HH
