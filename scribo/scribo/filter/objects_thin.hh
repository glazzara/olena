// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_FILTER_COMPONENTS_THIN_HH
# define SCRIBO_FILTER_COMPONENTS_THIN_HH

/// \file
///
/// \brief Remove too thin components.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/util/array.hh>

# include <mln/data/transform.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/filter/internal/compute.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /// \brief Remove components thinner or equal to \p min_thickness.
    ///
    /// \param[in] input_ a binary image.
    /// \param[in] nbh_ a neighborhood used in labeling algorithms.
    /// \param[in] label_type the label type used for labeling.
    /// \param[in] min_thickness the minimum thickness value.
    ///
    /// \result A binary image without thin components.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    components_thin(const Image<I>& input_,
		    const Neighborhood<N>& nbh_,
		    const V& label_type,
		    unsigned min_thickness);

    /// \brief Remove lines of text thinner or equal to \p
    /// min_thickness.
    ///
    /// \param[in] components An object image.
    /// \param[in] min_thickness the minimum thickness value.
    ///
    /// \result An object image without too thin components.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename L>
    inline
    component_set<L>
    components_thin(const component_set<L>& text,
		    unsigned min_thickness);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {


      /// Filter Functor.
      /// Return false for all components which are too large.
      template <typename L>
      struct components_thin_filter
	: Function_v2b< components_thin_filter<L> >
      {
	/// Constructor
	///
	/// \param[in] components object bounding boxes.
	/// \param[in] min_thickness the minimum thickness allowed.
	components_thin_filter(const component_set<L>& components,
			       unsigned min_thickness)
	  : components_(components), min_thickness_(min_thickness)
	{
	}


	/// Return false if the components is thinner than
	/// \p min_thickness_.
	///
	/// \param[in] l An image value.
	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return false;
	  return components_(l).bbox().nrows() > min_thickness_
	    && components_(l).bbox().ncols() > min_thickness_;
	}

	/// Component bounding boxes.
	component_set<L> components_;

	/// The minimum thickness.
	unsigned min_thickness_;
      };


    } //  end of namespace scribo::filter::internal


    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    components_thin(const Image<I>& input_,
		    const Neighborhood<N>& nbh_,
		    const V& label_type,
		    unsigned min_thickness)
    {
      trace::entering("scribo::filter::components_thin");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());
      (void) label_type;

      V nlabels;
      typedef mln_ch_value(I,V) lbl_t;
      component_set<lbl_t> comps
	  = primitive::extract::components(input, nbh, nlabels);

      typedef internal::components_thin_filter<lbl_t> func_t;
      func_t fv2b(comps, min_thickness);
      mln_concrete(I)
	output = mln::data::transform(comps.labeled_image(), fv2b);

      trace::exiting("scribo::filter::components_thin");
      return output;
    }


    template <typename L>
    inline
    component_set<L>
    components_thin(const component_set<L>& components,
		    unsigned min_thickness)
    {
      trace::entering("scribo::filter::components_thin");

      mln_precondition(components.is_valid());

      typedef internal::components_thin_filter<L> func_t;
      func_t is_not_too_thin(components, min_thickness);

      component_set<L> output = components.duplicate();
      output.update_tags(is_not_too_thin, component::Ignored);

      trace::exiting("scribo::filter::components_thin");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_COMPONENTS_THIN_HH
