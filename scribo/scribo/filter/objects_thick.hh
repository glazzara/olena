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

#ifndef SCRIBO_FILTER_OBJECTS_THICK_HH
# define SCRIBO_FILTER_OBJECTS_THICK_HH

/// \file
///
/// \brief Remove too thick components.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/util/array.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>



namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /// \brief Remove components thicker or equal to \p max_thickness.
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] nbh_ A neighborhood used in labeling algorithms.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result A binary image without thick components.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    objects_thick(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned max_thickness);


    /// \brief Remove components thicker or equal to \p max_thickness.
    ///
    /// \param[in] components An object image.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result An object image without too thick components.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename L>
    inline
    component_set<L>
    objects_thick(const component_set<L>& components,
		  unsigned max_thickness);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      /// Filter Functor. Return false for all components which are too
      /// large.
      template <typename L>
      struct thick_object_filter
	: Function_v2b< thick_object_filter<L> >
      {

	/// Constructor
	///
	/// \param[in] components An object image.
	/// \param[in] max_thickness the maximum thickness allowed.
	thick_object_filter(const component_set<L>& components,
			    unsigned max_thickness)
	  : components_(components), max_thickness_(max_thickness)
	{
	}


	/// Return false if the components is thicker than
	/// \p max_thickness_.
	///
	/// \param[in] l An image value.
	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return false;
	  return components_(l).bbox().nrows() < max_thickness_
	    && components_(l).bbox().ncols() < max_thickness_;
	}


	/// An object image.
	component_set<L> components_;

	/// The maximum thickness.
	unsigned max_thickness_;
      };


    } //  end of namespace scribo::filter::internal


    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    objects_thick(const Image<I>& input_,
		  const Neighborhood<N>& nbh_,
		  const V& label_type,
		  unsigned max_thickness)
    {
      trace::entering("scribo::filter::objects_thick");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      V nlabels;
      typedef mln_ch_value(I,V) lbl_t;
      component_set<lbl_t> components
	  = primitive::extract::components(input, nbh, nlabels);

      typedef internal::thick_object_filter<lbl_t> func_t;
      func_t fv2b(components, max_thickness);
      components.relabel(fv2b);

      mln_concrete(I) output = duplicate(input);
      data::fill((output | pw::value(components) == literal::zero).rw(), false);

      trace::exiting("scribo::filter::objects_thick");
      return output;
    }


    template <typename L>
    inline
    component_set<L>
    objects_thick(const component_set<L>& components,
		  unsigned max_thickness)
    {
      trace::entering("scribo::filter::objects_thick");

      mln_precondition(components.is_valid());

      typedef internal::thick_object_filter<L> func_t;
      func_t is_not_too_thick(components, max_thickness);

      component_set<L> output = components.duplicate();
      output.update_tags(is_not_too_thick, component::Ignored);

      trace::exiting("scribo::filter::objects_thick");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_THICK_HH
