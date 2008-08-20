// Copyright (C) 2006, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_OP_PREDEC_HH
# define MLN_TRAIT_OP_PREDEC_HH

/*! \file mln/trait/op/predec.hh
 *
 * \brief Declaration of the "unary pre-decrementation" operator trait.
 */

# include <mln/trait/op/decl.hh>


# define mln_trait_op_predec(T)  typename mln::trait::op::predec< T >::ret
# define mln_trait_op_predec_(T)          mln::trait::op::predec< T >::ret



namespace mln
{

  namespace trait
  {

    namespace op
    {

      template <typename T>
      struct predec : public solve_unary<predec, T>
      {
      };
      
    } // end of namespace mln::trait::op

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/solve.hh>


#endif // ! MLN_TRAIT_OP_PREDEC_HH
