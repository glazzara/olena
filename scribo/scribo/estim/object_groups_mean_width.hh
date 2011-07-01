// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_ESTIM_OBJECT_GROUPS_MEAN_WIDTH_HH
# define SCRIBO_ESTIM_OBJECT_GROUPS_MEAN_WIDTH_HH


/// \file
///
/// \brief Estimate the mean object width for each group.


# include <mln/util/array.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace estim
  {

    using namespace mln;

    /*! \brief Estimate the mean object width for each group.

      \param[in] groups Object groups information.

      \return An array of mean object width.

     */
    template <typename L>
    mln::util::array<float>
    object_groups_mean_width(const object_groups<L>& groups);



# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    mln::util::array<float>
    object_groups_mean_width(const object_groups<L>& groups)
    {
      trace::entering("scribo::estim::object_groups_mean_width");

      mln_precondition(groups.is_valid());

      const component_set<L>& components = groups.components();

      mln::util::array<float> output(groups.nelements(), 0.0);
      for_all_groups(g, groups)
	for_all_elements(e, groups(g).component_ids())
	  output(g) += components(groups(g).component_ids()(e)).bbox().width();

      output(0) = 0;
      for_all_groups(g, groups)
	if (groups(g).is_valid())
	  output(g) /= groups(g).card();
	else
	  output(g) = 0;

      trace::exiting("scribo::estim::object_groups_mean_width");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::estim

} // end of namespace scribo

#endif // ! SCRIBO_ESTIM_OBJECT_GROUPS_MEAN_WIDTH_HH
