// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_VALUE_CONCEPT_BUILT_IN_HH
# define MLN_VALUE_CONCEPT_BUILT_IN_HH

/*! \file mln/value/concept/built_in.hh
 *
 * \brief Define a generic class for built-in values.
 */

# include <mln/core/category.hh>


namespace mln
{

  namespace value
  {

    // Category flag type.
    template <>
    struct Built_In<void> // No inheritance here since this category is special (on the side).
    {
      // Every builtin belongs to a sub-category of Value but we do not know which one.
      typedef void* super;
    };

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_CONCEPT_BUILT_IN_HH
