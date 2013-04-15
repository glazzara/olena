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

#ifndef SCRIBO_FUN_V2B_COMPONENTS_ON_BORDER_FILTER_HH
# define SCRIBO_FUN_V2B_COMPONENTS_ON_BORDER_FILTER_HH

/// \file
///
/// \brief Remove components in image inner border.



# include <mln/core/concept/function.hh>

# include <mln/util/array.hh>

# include <mln/accu/math/count.hh>

# include <mln/labeling/compute.hh>

# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace fun
  {

    namespace v2b
    {

      using namespace mln;


      /// Filter Functor.
      /// Return false for all components which are too large.
      template <typename L>
      struct components_on_border_filter
	: Function_v2b< components_on_border_filter<L> >
      {
	typedef accu::math::count<mln_psite(L)> card_t;

	/// Constructor
	///
	/// \param[in] components Component bounding boxes.
	//
	components_on_border_filter(const component_set<L>& components);


	/// Check if the component is large enough.
	///
	/// \param[in] l A label.
	///
	/// \return false if the component area is strictly inferion to
	/// \p max_size_.
	//
	bool operator()(const mln_value(L)& l) const;

	/// The component set to filter.
	const component_set<L> components_;

	/// Labeled image bounding box.
	const box2d& b_;
      };




# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      inline
      components_on_border_filter<L>::components_on_border_filter(
	const component_set<L>& components)
	: components_(components), b_(components_.labeled_image().domain())
      {
      }



      template <typename L>
      inline
      bool
      components_on_border_filter<L>::operator()(const mln_value(L)& l) const
      {
	if (l == literal::zero)
	  return false;

	if (components_(l).bbox().pmin().row() == b_.pmin().row()
	    || components_(l).bbox().pmax().row() == b_.pmax().row()
	    || components_(l).bbox().pmin().col() == b_.pmin().col()
	    || components_(l).bbox().pmax().col() == b_.pmax().col())
	  return false;

	return true;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::fun::v2b

  } // end of namespace scribo::fun

} // end of namespace scribo

#endif // ! SCRIBO_FUN_V2B_COMPONENTS_ON_BORDER_FILTER_HH
