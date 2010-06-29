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

#ifndef SCRIBO_PRIMITIVE_INTERNAL_IS_LINK_VALID_HH
# define SCRIBO_PRIMITIVE_INTERNAL_IS_LINK_VALID_HH

/// \file
///
/// Validate a link from two different links.
///
/// \todo To be deleted ?.

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

namespace scribo
{

  namespace primitive
  {

    namespace internal
    {

      /// Validate a link from two different links.
      ///
      /// \param[in] left_link Left link of components.
      /// \param[in] right_link Right link of components.
      /// \param[in] i The component id.
      ///
      /// \return True if the link is between the \p i-th component
      /// and it neighbor is validated.
      template <typename L>
      mln::util::couple<bool,unsigned>
      is_link_valid(const object_links<L>& left_link,
		    const object_links<L>& right_link,
		    unsigned i);

# ifndef MLN_INCLUDE_ONLY

      template <typename L>
      mln::util::couple<bool,unsigned>
      is_link_valid(const object_links<L>& left_link,
		    const object_links<L>& right_link,
		    unsigned i)
      {
	bool b = (right_link(left_link(i)) == i && left_link(i) != i);
	return mln::make::couple(b, left_link(i));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::internal

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_INTERNAL_IS_LINK_VALID_HH
