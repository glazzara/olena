// Copyright (C) 2011, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PRIMITIVE_IDENTIFY_HH
# define SCRIBO_PRIMITIVE_IDENTIFY_HH

/*! \brief try to determine the type of a component.

\todo Write it and add support for component type (graphic, images,
...)
 */

# include <mln/debug/trace.hh>
# include <scribo/core/component_set.hh>

namespace scribo
{

  namespace primitive
  {

    template <typename L>
    component_set<L>
    identify(const component_set<L> comps)
    {
      mln_trace("scribo::primitive::identify");

      mln_assertion(comps.is_valid());

      // component_set<L> output = comps.duplicate();

      // for_all_comps(c, comps)
      // 	if (comps(c).is_valid())
      // 	{
      // 	  float
      // 	    min = comps(c).bbox().height(),
      // 	    max = comps(c).bbox().width();

      // 	  if (comps(c).bbox().width() < comps(c).bbox().height())
      // 	    std::swap(min, max);

      // 	  if (max/min > 10)
      // 	    output(c).update_type(component::LineSeparator);
      // 	}

      //return output;
      return comps;
    }

# ifndef MLN_INCLUDE_ONLY



# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_IDENTIFY_HH
