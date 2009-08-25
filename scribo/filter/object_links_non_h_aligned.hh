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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_NON_H_ALIGNED_HH
# define SCRIBO_FILTER_OBJECT_LINKS_NON_H_ALIGNED_HH

/// \file
///
/// Invalidate links between two non horizontaly aligned objects.


# include <mln/util/array.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_image.hh>
# include <scribo/filter/object_links_non_aligned.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /*! \brief Invalidate links between two non horizontaly aligned objects.
      Alignment is based on object bounding boxes.

      \param[in] objects   An object image.
      \param[in] links     Object links information.
      \param[in] max_delta Maximum delta.

      \result Filtered object links data.

      \verbatim
	                          v
                       ------ ~ ~ ~
                       |    |     |\
	  ------ ~ ~ ~ |~ ~ | ~ ~ ~ \
	  |    |       |    |     ^  \
	  | x- - - - - --x  |         => delta, must be < to max_delta
	  |    |       |    |     v  /
	  ------ ~ ~ ~ |~ ~ | ~ ~ ~ /
	  object1      |    |     |/
		       ------ ~ ~ ~
                      object2     ^

      \endverbatim


    */
    template <typename L>
    object_links<L>
    object_links_non_h_aligned(const object_image(L)& objects,
			       const object_links<L>& links,
			       float ratio);

# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_non_h_aligned(const object_image(L)& objects,
			       const object_links<L>& links,
			       float ratio)
    {
      trace::entering("scribo::filter::object_links_non_h_aligned");

      mln_precondition(objects.is_valid());

      object_links<L>
	output = object_links_non_aligned(objects, links, 1, ratio);

      trace::exiting("scribo::filter::object_links_non_h_aligned");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_LINKS_NON_H_ALIGNED_HH
