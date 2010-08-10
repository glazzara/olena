// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_FILTER_OBJECT_GROUPS_V_THICKNESS_HH
# define SCRIBO_FILTER_OBJECT_GROUPS_V_THICKNESS_HH

/// \file
///
/// \brief Filter groups having their object mean thickness too low.


# include <mln/util/array.hh>

# include <scribo/core/object_groups.hh>
# include <scribo/estim/object_groups_v_thickness.hh>


namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /*! \brief Filter groups having their object mean thickness too low.

      \param[in] groups    Object group information.
      \param[in] thickness Object group mean thickness must be greater
      or equal to this value.

      \return Filtered object group information.

     */
    template <typename L>
    object_groups<L>
    object_groups_v_thickness(const object_groups<L>& groups, float thickness);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    object_groups<L>
    object_groups_v_thickness(const object_groups<L>& groups, float thickness)
    {
      trace::entering("scribo::filter::object_groups_v_thickness");

      mln_precondition(groups.is_valid());
      mln_precondition(thickness >= 0);

      mln::util::array<float>
	group_thickness = estim::object_groups_v_thickness(groups);

      object_groups<L> output = groups.duplicate();
      output(0) = 0;
      for (unsigned i = 1; i < output.nelements(); ++i)
	if (groups.components()(i).is_valid()
	    && group_thickness[groups(i)] < thickness)
	  output(i) = 0;

      trace::exiting("scribo::filter::object_groups_v_thickness");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::filter

} // end of namespace scribo


#endif // ! SCRIBO_FILTER_OBJECT_GROUPS_V_THICKNESS_HH
