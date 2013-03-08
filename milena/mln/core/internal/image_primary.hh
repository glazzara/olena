// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_INTERNAL_IMAGE_PRIMARY_HH
# define MLN_CORE_INTERNAL_IMAGE_PRIMARY_HH

/*! \file
 *
 * \brief Definition of a base class for primary images.
 */

# include <mln/core/internal/image_base.hh>


namespace mln
{

  namespace internal
  {


    /*!
      \internal
      \brief A base class for primary images.
    */
    template <typename T, typename S, typename E>
    struct image_primary : public image_base<T, S, E>
    {
    protected:
      image_primary();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename S, typename E>
    inline
    image_primary<T, S, E>::image_primary()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_PRIMARY_HH
