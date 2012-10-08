// Copyright (C) 2006, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_NEXT_SOLVE_HH
# define MLN_TRAIT_NEXT_SOLVE_HH

/// \file
///
/// FIXME
 
# include <mln/core/category.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/ret.hh>
# include <mln/trait/solve.hh>


// FIXME: Just for the record (use it...)

#  ifndef MLN_DEBUG_TRAITS
#  endif // ! MLN_DEBUG_TRAITS


# define mln_trait_nunary(Name, T)  typename mln::trait::next::solve_unary< Name, T >::ret
# define mln_trait_nunary_(Name, T)          mln::trait::next::solve_unary< Name, T >::ret

# define mln_trait_nbinary(Name, T1, T2)  typename mln::trait::next::solve_binary< Name, T1, T2 >::ret
# define mln_trait_nbinary_(Name, T1, T2)          mln::trait::next::solve_binary< Name, T1, T2 >::ret



namespace mln
{

  namespace trait
  {

    namespace next
    {

      // Unary case.


      template < typename Name,
		 typename T >
      struct set_precise_unary_
      {
	typedef undefined ret;
      };


      template < typename Name,
		 template <class> class Category_T, typename T >
      struct set_unary_
      {
	typedef undefined ret;
      };

      template < typename Name,
		 typename T >
      struct set_unary_< Name, Unknown, T > // Blocker; top of inheritance.
      {
	typedef not_found ret;
      };




      // Binary case.


      template < typename Name,
		 typename L,
		 typename R >
      struct set_precise_binary_
      {
	typedef undefined ret;
      };


      template < typename Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct set_binary_
      {
	typedef undefined ret;
      };

      template < typename Name,
		 typename L,
		 template <class> class Category_R, typename R >
      struct set_binary_< Name, Unknown, L, Category_R, R > // Left blocker.
      {
	typedef not_found ret;
      };

      template < typename Name,
		 template <class> class Category_L, typename L,
		 typename R >
      struct set_binary_< Name, Category_L, L, Unknown, R > // Right blocker.
      {
	typedef not_found ret;
      };

      template < typename Name,
		 typename L,
		 typename R >
      struct set_binary_< Name, Unknown, L, Unknown, R > // Blocker.
      {
	typedef not_found ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/trait/next/solve_unary.hh>
# include <mln/trait/next/solve_binary.hh>
# include <mln/trait/next/solve_proxy.hh>

#endif // ! MLN_TRAIT_NEXT_SOLVE_HH
