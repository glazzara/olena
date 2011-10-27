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

#ifndef SCRIBO_FILTER_OBJECT_LINKS_BOTTOM_ALIGNED_HH
# define SCRIBO_FILTER_OBJECT_LINKS_BOTTOM_ALIGNED_HH

/// \file
///
/// \brief Invalidate links between two components if their bottom are
/// not aligned.


# include <mln/util/array.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>

# include <scribo/filter/object_links_aligned.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;

    /*! \brief Invalidate links between two components if their bottom are not
               aligned.

	\param[in] links        Object links information.
        \param[in] max_alpha    Maximum angle value (degrees).

	\verbatim


                       ------
                       |    |
	  ------       |    |
	  |    |       |    |
	  | x------------x  |
	  |    |       |    |
	  ------~ ~ ~ ~| ~ ~| ~ ~
	 object1  ~    |    |   ^
	            ~  |    |   |
		      ~------   |
                      object2   | Alpha
                        ~       |
			  ~     |
			    ~   |
			      ~ v
			        ~
	\endverbatim

	The angle between the two bottoms must be lower than \p max_alpha.

	\ingroup grpalgofiltercomplink
    */
    template <typename L>
    object_links<L>
    object_links_bottom_aligned(const object_links<L>& links,
				float max_alpha);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_links<L>
    object_links_bottom_aligned(const object_links<L>& links,
				float max_alpha)
    {
      trace::entering("scribo::filter::object_links_bottom_aligned");

      mln_precondition(links.is_valid());

      object_links<L>
	output = object_links_aligned(links, max_alpha, anchor::Bottom);

      trace::exiting("scribo::filter::object_links_bottom_aligned");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_LINKS_BOTTOM_ALIGNED_HH
