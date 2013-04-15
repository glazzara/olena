// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FUN_V2B_COMPONENTS_LARGE_FILTER_HH
# define SCRIBO_FUN_V2B_COMPONENTS_LARGE_FILTER_HH

/// \file
///
/// Remove large components in a binary image.



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
      struct components_large_filter
	: Function_v2b< components_large_filter<L> >
      {
	typedef accu::math::count<mln_psite(L)> card_t;

	/// Constructor
	///
	/// \param[in] components Component bounding boxes.
	/// \param[in] max_size Maximum component size.
	//
	components_large_filter(const component_set<L>& components,
				unsigned max_size);


	/// Check if the component is large enough.
	///
	/// \param l A label.
	///
	/// \return false if the component area is strictly inferion to
	/// \p max_size_.
	//
	bool operator()(const mln_value(L)& l) const;

	/// The minimum area.
	unsigned max_size_;

	/// The component set to filter.
	const component_set<L> components_;

	/// The number of labels remaining after filtering.
	mutable mln_value(L) nlabels_;

	/// Has already been taken into account.
	mutable mln::util::array<bool> marked_;
      };




# ifndef MLN_INCLUDE_ONLY


      template <typename L>
      inline
      components_large_filter<L>::components_large_filter(
	const component_set<L>& components,
	unsigned max_size)
	: max_size_(max_size), components_(components), nlabels_(0),
	  marked_(mln::value::next(components.nelements()), false)
      {
      }



      template <typename L>
      inline
      bool
      components_large_filter<L>::operator()(const mln_value(L)& l) const
      {
	if (l == literal::zero)
	  return false;
	if (components_.info(l).card() <= max_size_)
	{
	  if (!marked_(l))
	  {
	    nlabels_ = value::next(nlabels_);
	    marked_(l) = true;
	  }
	  return true;
	}

	return false;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::fun::v2b

  } // end of namespace scribo::fun

} // end of namespace scribo

#endif // ! SCRIBO_FUN_V2B_COMPONENTS_LARGE_FILTER_HH
