// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_LITERAL_GRAYS_HH
# define MLN_LITERAL_GRAYS_HH

/*! \file
 * \brief Definition of the grays literal.
 *
 */

# include <mln/core/concept/literal.hh>

namespace mln
{

  namespace literal
  {

    /// Type of literal grays.
    struct light_gray_t : public Literal<light_gray_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      light_gray_t();
    };

    struct medium_gray_t : public Literal<medium_gray_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      medium_gray_t();
    };

    struct dark_gray_t : public Literal<dark_gray_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      dark_gray_t();
    };

    /// Literal light gray.
    extern const light_gray_t light_gray;

    /// Literal medium_gray.
    extern const medium_gray_t medium_gray;

    /// Literal dark gray.
    extern const dark_gray_t dark_gray;

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    const light_gray_t light_gray;

    const medium_gray_t medium_gray;

    const dark_gray_t dark_gray;

#  endif // ! MLN_WO_GLOBAL_VARS

    inline
    light_gray_t::light_gray_t()
    {
    }

    inline
    medium_gray_t::medium_gray_t()
    {
    }

    inline
    dark_gray_t::dark_gray_t()
    {
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln

// White and black are grays too.
# include <mln/literal/white.hh>
# include <mln/literal/black.hh>

#endif // ! MLN_LITERAL_GRAYS_HH
