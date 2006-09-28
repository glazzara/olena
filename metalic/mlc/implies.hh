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

#ifndef MLC_IMPLIES_HH
# define MLC_IMPLIES_HH

# include <mlc/bexpr.hh>
# include <mlc/assert.hh>
# include <mlc/is_a.hh>


/*! \def mlc_implies(L, R)
**
** Macro corresponding to mlc::implies_<L, R>, for use in a template
** context.
**
** \see mlc::implies_<L, R>
*/

# define mlc_implies(L, R)  mlc::implies_< L, R >


namespace mlc
{

  namespace ERROR
  {
    struct FIRST_PARAMETER_OF_mlc_implies_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_implies_SHOULD_BE_A_BEXPR;

  } // end of namespace mlc::ERROR


  /*! \class mlc::implies_<L, R>
  **
  ** Logical implication "L => R" with L and R being Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Sample use:
  **   mlc::assert_< mlc::implies_< mlc_is_builtin(T), mlc_eq(T, int) > >::check();
  ** which means "if T is a buit-in type, it has to be int".
  */

  template <typename L, typename R>
  struct implies_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_implies_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_implies_SHOULD_BE_A_BEXPR >,

      public mlc::bexpr_<( !mlc_bool(L) || mlc_bool(R) )>
  {};

} // end of namespace mlc



#endif // ! MLC_IMPLIES_HH
