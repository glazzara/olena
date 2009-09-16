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

#ifndef SCRIBO_FILTER_COMMON_OBJECTS_PHOTO_HH
# define SCRIBO_FILTER_COMMON_OBJECTS_PHOTO_HH

/// \file
///
/// \brief Common objects filters to use in photos.


# include <scribo/core/object_image.hh>

# include <scribo/filter/objects_small.hh>
# include <scribo/filter/objects_thin.hh>
# include <scribo/filter/objects_thick.hh>


namespace scribo
{

  namespace filter
  {

    namespace common
    {


      /*! \brief Common objects filters to use in photos.

	\param[in] objects An object image.

	\return A filtered object image.
       */
      template <typename L>
      object_image(L)
      objects_photo(const object_image(L)& objects);


# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      object_image(L)
      objects_photo(const object_image(L)& objects)
      {
	trace::entering("scribo::filter::common::objects_photo");

	mln_precondition(objects.is_valid());


	object_image(L) filtered_objects
	  = scribo::filter::objects_small(objects, 6);

	filtered_objects
	  = scribo::filter::objects_thin(filtered_objects, 1);

// 	filtered_objects
// 	  = scribo::filter::objects_thick(filtered_objects,
// 					  math::min(input.ncols(),
// 						    input.nrows()) / 5);

	trace::exiting("scribo::filter::common::objects_photo");
	return filtered_objects;
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::filter::common

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_COMMON_OBJECTS_PHOTO_HH
