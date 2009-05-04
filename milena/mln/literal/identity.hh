// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_LITERAL_IDENTITY_HH
# define MLN_LITERAL_IDENTITY_HH

/// \file mln/literal/identity.hh
///
/// Definition of the literal of mln::identity.

# include <mln/core/concept/literal.hh>


namespace mln
{

  namespace literal
  {

    /// Type of literal identity.
    struct identity_t : public Literal<identity_t>
    {
    };

    /// Literal identity.
    extern const identity_t& identity;

# ifndef MLN_INCLUDE_ONLY

    const identity_t& identity = identity_t();

# endif

  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_IDENTITY_HH