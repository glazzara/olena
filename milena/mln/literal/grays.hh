// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_LITERAL_GRAYS_HH
# define MLN_LITERAL_GRAYS_HH

/*! \file
 * \brief Definition of the colors literal.
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
    };

    struct medium_gray_t : public Literal<medium_gray_t>
    {
    };

    struct dark_gray_t : public Literal<dark_gray_t>
    {
    };

    /// Literal light gray.
    extern const light_gray_t& light_gray;

    /// Literal medium_gray.
    extern const medium_gray_t& medium_gray;

    /// Literal dark gray.
    extern const dark_gray_t& dark_gray;

# ifndef MLN_INCLUDE_ONLY

    const light_gray_t& light_gray = light_gray_t();

    const medium_gray_t& medium_gray = medium_gray_t();

    const dark_gray_t& dark_gray = dark_gray_t();

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln

// White and black are grays too.
# include <mln/literal/white.hh>
# include <mln/literal/black.hh>

#endif // ! MLN_LITERAL_GRAYS_HH
