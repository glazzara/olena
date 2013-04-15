// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_FILTER_COMMON_COMPONENTS_PHOTO_HH
# define SCRIBO_FILTER_COMMON_COMPONENTS_PHOTO_HH

/// \file
///
/// \brief Common components filters to use in photos.


# include <scribo/core/component_set.hh>

# include <scribo/filter/objects_small.hh>
# include <scribo/filter/objects_thin.hh>
# include <scribo/filter/objects_thick.hh>


namespace scribo
{

  namespace filter
  {

    namespace common
    {


      /*! \brief Common components filters to use in photos.

	\param[in] components An object image.

	\return A filtered object image.
       */
      template <typename L>
      component_set<L>
      components_photo(const component_set<L>& components);


# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      component_set<L>
      components_photo(const component_set<L>& components)
      {
	mln_trace("scribo::filter::common::components_photo");

	mln_precondition(components.is_valid());


	component_set<L> filtered_components
	  = scribo::filter::components_small(components, 6);

	filtered_components
	  = scribo::filter::components_thin(filtered_components, 1);

// 	filtered_components
// 	  = scribo::filter::components_thick(filtered_components,
// 					  math::min(input.ncols(),
// 						    input.nrows()) / 5);

	return filtered_components;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::filter::common

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_COMMON_COMPONENTS_PHOTO_HH
