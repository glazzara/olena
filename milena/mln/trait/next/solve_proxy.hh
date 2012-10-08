// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_NEXT_SOLVE_PROXY_HH
# define MLN_TRAIT_NEXT_SOLVE_PROXY_HH

/// \file
///
/// Proxy support for "next" trait solver.


# include <mln/core/concept/object.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/proxy.hxx>

# include <mln/trait/next/solve.hh>

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
      template < typename Name, typename T_ > struct solve_unary;



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
