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

#ifndef SCRIBO_FILTER_COMPONENTS_ON_BORDER_HH
# define SCRIBO_FILTER_COMPONENTS_ON_BORDER_HH

/// \file
///
/// \brief Remove components located on image borders.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/function.hh>

# include <mln/labeling/compute.hh>
# include <mln/accu/math/count.hh>

# include <mln/util/array.hh>

# include <mln/pw/all.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/fun/v2b/objects_on_border_filter.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /// \brief Remove components located on image borders.
    ///
    /// \param[in] components    An object image.
    ///
    /// \return A component set with large components set to
    /// component::Ignored.
    ///
    /// \ingroup grpalgofiltercomp
    //
    template <typename L>
    inline
    component_set<L>
    components_on_border(const component_set<L>& components);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    inline
    component_set<L>
    components_on_border(const component_set<L>& components)
    {
      mln_trace("scribo::filter::components_on_border");

      mln_precondition(components.is_valid());

      fun::v2b::components_on_border_filter<L> f(components);

      component_set<L> output = components.duplicate();
      output.update_tags(f, component::Ignored);

      return output;
    }



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_COMPONENTS_ON_BORDER_HH
