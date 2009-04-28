// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_NEXT_SOLVE_PROXY_HH
# define MLN_TRAIT_NEXT_SOLVE_PROXY_HH

/*! \file mln/trait/next/solve_proxy.hh
 *
 * \brief Proxy support for "next" trait solver.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/proxy.hxx>

namespace mln
{

  namespace trait
  {

    namespace next
    {

      // Forward declaration
      // Needed by mln_trait_nbinary.
      template < typename Name, typename L_, typename R_ >
      struct solve_binary;



      // Unary ops.

      template < typename Op, typename P >
      struct set_unary_< Op, mln::Proxy, P >
      {
	typedef mlc_unqualif(mln_q_subject(P)) S;
	typedef mln_trait_nunary(Op, S) ret;
      };

      // Binary ops.

      template < typename Op, typename L, typename R >
      struct set_binary_< Op, mln::Proxy, L, mln::Proxy, R >
      {
	typedef mln::internal::helper_unprox_binop<L, R> helper;
	typedef mln_trait_nbinary(Op,
				  typename helper::L_ret,
				  typename helper::R_ret) ret;
      };

      template < typename Op, typename P, typename O >
      struct set_binary_< Op, mln::Proxy, P, mln::Object, O >
      {
	typedef mlc_unqualif(mln_q_subject(P)) S;
	typedef mln_trait_nbinary(Op, S, O) ret;
      };

      template < typename Op, typename O, typename P >
      struct set_binary_< Op, mln::Object, O, mln::Proxy, P  >
      {
	typedef mlc_unqualif(mln_q_subject(P)) S;
	typedef mln_trait_nbinary(Op, O, S) ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_NEXT_SOLVE_PROXY_HH
