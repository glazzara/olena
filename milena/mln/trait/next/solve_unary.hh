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

#ifndef MLN_TRAIT_NEXT_SOLVE_UNARY_HH
# define MLN_TRAIT_NEXT_SOLVE_UNARY_HH

/*!
 * \file
 *
 * \brief  FIXME
 *
 *
 */

# include <mln/core/category.hh>
# include <mln/core/routine/exact.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/ret.hh>
# include <mln/trait/next/solve.hh>


// FIXME: Just for the record (use it...)

#  ifndef MLN_DEBUG_TRAITS
#  endif // ! MLN_DEBUG_TRAITS



namespace mln
{

  namespace trait
  {

    namespace next
    {

      namespace internal
      {


	template < typename Name,
		   typename Category,
		   typename T >
	struct trait_set_unary_;

	template < typename Name,
		   template <class> class Category, typename _,
		   typename T >
	struct trait_set_unary_< Name, Category<_>, T >
	{
	  typedef typename mln::trait::next::set_unary_<Name, Category, T>::ret ret;
	};


	// Fwd decls.
	template < typename Name,
		   typename Category, typename T >
	struct get_unary_;


	template < typename user_ret, /* != not_found and != undefined */
		   typename Name,
		   typename Category, typename T >
	struct helper_get_unary_
	{
	  typedef user_ret ret;  // The user has defined 'ret' so we return it.
	};


	template < typename Name,
		   typename Category, typename T >
	struct helper_get_unary_< /* user_ret == */ not_found,
				  Name, Category, T >
	{
	  typedef not_found ret;  // End of search due to a blocker; 'ret' is not found.
	};


	template < typename Name,
		   typename Category, typename T >
	struct helper_get_unary_< /* user_ret == */ undefined,
				  Name, Category, T >
	{
	  typedef typename mln::internal::super_category_< Category, T >::ret Super_Category;
	  typedef typename get_unary_<Name, Super_Category, T>::ret ret;   // No user ret definition => Recursion.
	};


	template < typename Name,
		   typename Category, typename T >
	struct get_unary_
	{
	  typedef typename trait_set_unary_<Name, Category, T>::ret user_ret;  // First get 'user_ret'
	  typedef helper_get_unary_<user_ret, Name, Category, T> helper;       // Set the helper to make a decision.
	  typedef mlc_ret(helper) ret;                                         // Return.
	};


	template < typename precise_ret,
		   typename Name,
		   typename Category, typename T >
	struct helper_choose_unary_wrt_ /* precise_ret != undefined */
	{
	  typedef precise_ret ret;                                 // -> A precise ret has been defined so it is it.
	};

	template < typename Name,
		   typename Category, typename T >
	struct helper_choose_unary_wrt_< /* precise_ret == */ undefined,
					Name, Category, T >
	{
	  typedef typename get_unary_<Name, Category, T>::ret ret; // -> Go up into the category inheritance
								  //    to fetch a ret from 'set_unary_'s.
	};

	template < typename Name,
		   typename Category, typename T >
	struct helper_solve_unary_
	{
	  typedef typename set_precise_unary_<Name, T>::ret precise_ret;
	  typedef helper_choose_unary_wrt_< precise_ret, /* undefined or not (?) */
					    Name, Category, T> helper;
	  typedef mlc_ret(helper) ret;
	};

      } // end of namespace mln::trait::next::internal


      template < typename Name,
		typename T_ >
      struct solve_unary
      {
	typedef mln_exact(T_) T;
	typedef typename mln::category<T>::ret Category;
	typedef internal::helper_solve_unary_< Name, Category, T > meta_code;
	typedef typename meta_code::ret ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_NEXT_SOLVE_UNARY_HH
