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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_INIT_LINK_ARRAY_HH
# define SCRIBO_PRIMITIVE_INTERNAL_INIT_LINK_ARRAY_HH

/// \file
///
/// Initialize a link array.


# include <scribo/core/object_links.hh>
# include <scribo/core/component_set.hh>


namespace scribo
{

  using namespace mln;

  namespace primitive
  {

    namespace internal
    {

      /// Initialize a link array.
      template <typename L>
      void
      init_link_array(object_links<L>& link_array);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      inline
      void
      init_link_array(object_links<L>& link_array)
      {
	const component_set<L>& comp_set = link_array.component_set_();
	for (unsigned i = 0; i < link_array.nelements(); ++i)
	  if (comp_set(i).tag() == component::Ignored)
	    link_array[i] = 0;
	  else
	    link_array[i] = i;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_INTERNAL_INIT_LINK_ARRAY_HH
