// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_LITERAL_WHITE_HH
# define MLN_LITERAL_WHITE_HH

/// \file
/// \brief Definition of the 'white' literal.

# include <mln/core/concept/literal.hh>


namespace mln
{

  namespace literal
  {

    /// Type of literal white.
    struct white_t : public Literal<white_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      white_t();

# ifdef MLN_NEW_VALUE_TYPES
      operator float()  const;
      operator double() const;
# endif // MLN_NEW_VALUE_TYPES
    };


    /// Literal white.
    extern const white_t white;


# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    const white_t white;

#  endif

    white_t::white_t()
    {
    }

#  ifdef MLN_NEW_VALUE_TYPES
    inline
    white_t::operator float() const
    {
      return 1.f;
    }

    inline
    white_t::operator double() const
    {
      return 1.;
    }
#  endif // MLN_NEW_VALUE_TYPES

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_WHITE_HH
