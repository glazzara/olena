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

#ifndef SCRIBO_FILTER_OBJECTS_V_THICK_HH
# define SCRIBO_FILTER_OBJECTS_V_THICK_HH

/// \file
///
/// Remove too verticaly thick components.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <scribo/core/component_set.hh>
# include <scribo/filter/internal/compute.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /// Remove components verticaly thicker or equal to \p max_thickness.
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] nbh_ A neighborhood used in labeling algorithms.
    /// \param[in] label_type The label type used for labeling.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result A binary image without thick components.
    //
    template <typename I, typename N, typename V>
    inline
    mln_concrete(I)
    objects_v_thick(const Image<I>& input_,
		    const Neighborhood<N>& nbh_,
		    const V& label_type,
		    unsigned max_thickness);


    /// Remove components verticaly thicker or equal to \p max_thickness.
    ///
    /// \param[in] comps A component set.
    /// \param[in] max_thickness The maximum thickness value.
    ///
    /// \result An object image without too thick components.
    //
    template <typename L>
    inline
    component_set<L>
    objects_v_thick(const component_set<L>& comps,
		    unsigned max_thickness);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /// Filter Functor. Return false for all components which are too
      /// large.
      template <typename L>
      struct v_thick_object_filter
	: Function_v2b< v_thick_object_filter<L> >
      {

	/// Constructor
	///
	/// \param[in] comps A component set.
	/// \param[in] max_thickness the maximum thickness allowed.
	//
	v_thick_object_filter(const component_set<L>& comps,
			      unsigned max_thickness)
	  : comps_(comps), max_thickness_(max_thickness)
	{
	}

	/// Constructor
	///
	/// \param[in] max_thickness the maximum thickness allowed.
	//
	v_thick_object_filter(unsigned max_thickness)
	  : max_thickness_(max_thickness)
	{
	}

	/// Set the underlying component set.
	//
	void update_objects(const component_set<L>& comps)
	{
	  comps_ = comps;
	}

	/// Return false if the components is thicker than
	/// \p max_thickness_.
	///
	/// \param[in] l An image value.
	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return false;
	  return comps_(l).bbox().height() < max_thickness_;
	}


	/// A component set.
	component_set<L> comps_;

	/// The min thickness.
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
      trace::entering("scribo::filter::objects_v_thick");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mln_precondition(input.is_valid());
      mln_precondition(nbh.is_valid());

      internal::v_thick_object_filter<V> functor(max_thickness);
      mln_concrete(I)
	output = internal::compute(input, nbh, label_type, functor);

      trace::exiting("scribo::filter::objects_v_thick");
      return output;
    }


    template <typename L>
    inline
    component_set<L>
    objects_v_thick(const component_set<L>& comps,
		    unsigned max_thickness)
    {
      trace::entering("scribo::filter::objects_v_thick");

      internal::v_thick_object_filter<L> functor(comps, max_thickness);
      component_set<L> output = internal::compute(comps, functor);

      trace::exiting("scribo::filter::objects_v_thick");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_V_THICK_HH
