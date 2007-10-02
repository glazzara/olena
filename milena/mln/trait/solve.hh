// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef MLN_TRAIT_SOLVE_HH
# define MLN_TRAIT_SOLVE_HH

# include <mln/core/category.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>



namespace mln
{

  namespace trait
  {


    struct undefined;


    // Unary case.


    template < template <class> class Name,
	       typename T >
    struct set_precise_unary_
    {
      typedef undefined ret;
    };


    template < template <class> class Name,
	       template <class> class Category_T, typename T >
    struct set_unary_
    {
      typedef undefined ret;
    };
    

    namespace internal
    {

      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_choose_unary_
      {
	typedef typename set_unary_<Name, Category_T,T>::ret category_ret;
	typedef typename set_precise_unary_<Name, T>::ret precise_ret;
	typedef mlc_if( mlc_equal(precise_ret,
				  undefined),
			category_ret,
			precise_ret ) ret;
      };

      template < template <class> class Name,
		 typename Category_T_void, typename T >
      struct helper_solve_unary_;

      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_solve_unary_< Name,
				  Category_T<void>, T > : helper_choose_unary_< Name,
										Category_T, T >
      {
      };

    } // end of namespace mln::trait::internal


    template < template <class> class Name,
	       typename T >
    struct solve_unary : internal::helper_solve_unary_< Name,
							typename mln::category<T>::ret, T >
    {
    };



    // Binary case.
    

    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_
    {
      typedef undefined ret;
    };


    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct set_precise_binary_
    {
      typedef undefined ret;
    };


    namespace internal
    {

      template < template <class, class> class Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct helper_choose_binary_
      {
	typedef typename set_binary_<Name, Category_L,L, Category_R,R>::ret category_ret;
	typedef typename set_precise_binary_<Name, L, R>::ret precise_ret;
	typedef mlc_if( mlc_equal(precise_ret,
				  undefined),
			category_ret,
			precise_ret ) ret;
      };

      template < template <class, class> class Name,
		 typename Category_L_void, typename L,
		 typename Category_R_void, typename R >
      struct helper_solve_binary_;

      template < template <class, class> class Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct helper_solve_binary_< Name,
				   Category_L<void>, L,
				   Category_R<void>, R > : helper_choose_binary_< Name,
										  Category_L, L,
										  Category_R, R >
      {
      };

    } // end of namespace mln::trait::internal


    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct solve_binary : internal::helper_solve_binary_< Name,
							  typename mln::category<L>::ret, L,
							  typename mln::category<R>::ret, R >
    {
    };


  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_SOLVE_HH
