// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef METALIC_IMPLIES_HH
# define METALIC_IMPLIES_HH

# include <mlc/bool.hh>


/*! \def mlc_implies(Left_BExpr, Right_BExpr)
**
** Macro corresponding to mlc::implies_<L, R>, for use in a template
** context.
**
** \see mlc::implies_<L, R>
*/

# define mlc_implies(Left_BExpr, Right_BExpr) \
   typename mlc::implies_<Left_BExpr, Right_BExpr>::ret

# define mlc_implies_(Left_BExpr, Right_BExpr) \
   mlc::implies_<Left_BExpr, Right_BExpr>::ret


namespace mlc
{

  /*! \class mlc::implies_<L, R>
  **
  ** Logical implication "L => R" with L and R being Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Sample use:
  **   mlc::implies_< mlc_is_builtin(T), mlc_eq(T, int) >::ensure();
  ** which means "if T is a buit-in type, it has to be int".
  */

  template <typename L, typename R>
  struct implies_
    : public bool_<( !mlc_bool(L) || mlc_bool(R) )>
  {};

} // end of namespace mlc



#endif // ! METALIC_IMPLIES_HH
