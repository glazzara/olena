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

#ifndef SCRIBO_FILTER_OBJECTS_SIZE_RATIO_HH
# define SCRIBO_FILTER_OBJECTS_SIZE_RATIO_HH

/// \file
///
/// \brief Invalidate components with a width/height ratio too low.

# include <mln/core/concept/function.hh>

# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace filter
  {
    using namespace mln;

    /// \brief Invalidate components with a height/width ratio too low.
    ///
    /// Compute the ratio height/width from the component bounding
    /// boxes and compare it to \p size_ratio.
    ///
    /// If the height/width ratio is lower than min_size_ratio then
    /// the component is invalidated.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename L>
    component_set<L>
    objects_size_ratio(const component_set<L>& comps,
		       float min_size_ratio);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      /// Filter Functor.
      /// Return false for all components having  a too low size ratio.
      //
      template <typename L>
      struct components_size_ratio_filter
	: Function_v2b< components_size_ratio_filter<L> >
      {
	components_size_ratio_filter(const component_set<L>& comps,
				  float min_size_ratio)
	  : comps_(comps), min_size_ratio_(min_size_ratio)
	{
	}

	bool operator()(const mln_value(L)& l) const
	{
	  if (l == literal::zero)
	    return false;
	  return (comps_(l).bbox().height() / static_cast<float>(comps_(l).bbox().width())) >= min_size_ratio_;
	}

	/// Components data.
	component_set<L> comps_;

	/// The minimum size ratio.
	float min_size_ratio_;
      };


    } //  end of namespace scribo::filter::internal



    template <typename L>
    component_set<L>
    objects_size_ratio(const component_set<L>& comps,
		       float min_size_ratio)
    {

      mln_trace("scribo::primitive::objects_size_ratio");

      typedef internal::components_size_ratio_filter<L> func_t;
      func_t has_valid_size_ratio(comps, min_size_ratio);

      component_set<L> output = comps.duplicate();
      output.update_tags(has_valid_size_ratio, component::Ignored);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECTS_SIZE_RATIO_HH
