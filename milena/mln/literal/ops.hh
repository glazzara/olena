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

#ifndef MLN_LITERAL_OPS_HH
# define MLN_LITERAL_OPS_HH

/*! \file mln/literal/ops.hh
 * \brief Definitions of some operators.
 */

# include <mln/core/concept/literal.hh>



namespace mln
{

  template <typename O, typename L>
  bool operator==(const Object<O>& lhs, const Literal<L>& rhs);

  template <typename L, typename O>
  bool operator==(const Literal<L>& lhs, const Object<O>& rhs);

  template <typename L1, typename L2>
  bool operator==(const Literal<L1>& lhs, const Literal<L2>& rhs);

  template <typename L>
  bool operator==(const Literal<L>& lhs, const Literal<L>& rhs);

  // FIXME: Add other operators... 



# ifndef MLN_INCLUDE_ONLY

  template <typename O, typename L>
  bool operator==(const Object<O>& lhs, const Literal<L>& rhs)
  {
    return exact(lhs) == O(exact(rhs));
  }

  template <typename L, typename O>
  bool operator==(const Literal<L>& lhs, const Object<O>& rhs)
  {
    return rhs == lhs;
  }

  template <typename L1, typename L2>
  bool operator==(const Literal<L1>&, const Literal<L2>&)
  {
    return false;
  }

  template <typename L>
  bool operator==(const Literal<L>&, const Literal<L>&)
  {
    return true;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_LITERAL_OPS_HH
