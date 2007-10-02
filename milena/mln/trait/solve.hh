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


    /// Flag type for an undefined trait.
    struct undefined;

    /// Flag type for a trait multiply undefined.
    struct multiply_defined;


    // Unary case.


    template < template <class> class Name,
	       typename T >
    struct set_precise_unary_
    {
      typedef undefined ret;
    };


    template < template <class> class Name,
	       template <class> class Category_T, typename T >
    struct set_unary_;  // Fwd decl.


    template < template <class> class Name,
	       typename T >
    struct set_unary_< Name, Unknown, T > // Blocker; top of inheritance.
    {
      typedef undefined ret;
    };


    namespace internal
    {

      template < template <class> class Name,
		 typename Super_Category_T, typename T >
      struct set_unary_super_;

      template < template <class> class Name,
		 template <class> class Super_Category_T, typename T >
      struct set_unary_super_< Name, Super_Category_T<void>, T >
	:
	public set_unary_< Name, Super_Category_T, T >
      {
      };

    } // end of namespace mln::trait::internal


    template < template <class> class Name,
	       template <class> class Category_T, typename T >
    struct set_unary_ : internal::set_unary_super_< Name,
						    typename Category_T<void>::super, T >
    {
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
	       typename L,
	       typename R >
    struct set_precise_binary_
    {
      typedef undefined ret;
    };
    

    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_;  // Fwd decl.


    template < template <class, class> class Name,
	       typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_< Name, Unknown, L, Category_R, R > // Left blocker.
    {
      typedef undefined ret;
    };


    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       typename R >
    struct set_binary_< Name, Category_L, L, Unknown, R > // Right blocker.
    {
      typedef undefined ret;
    };


    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct set_binary_< Name, Unknown, L, Unknown, R > // Blocker.
    {
      typedef undefined ret;
    };


    namespace internal
    {

      // Nota bene: Seq means "super or equal".
      template < template <class, class> class Name,
		 typename Seq_Category_L, typename L,
		 typename Seq_Category_R, typename R >
      struct set_binary_super_;

      template < template <class, class> class Name,
		 template <class> class Seq_Category_L, typename L,
		 template <class> class Seq_Category_R, typename R >
      struct set_binary_super_< Name,
				Seq_Category_L<void>, L,
				Seq_Category_R<void>, R >
	:
	public virtual set_binary_< Name,
				    Seq_Category_L, L,
				    Seq_Category_R, R >
      {
      };

      template < typename L_ret, typename R_ret >
      struct merge_binary_ret_
      {
	typedef multiply_defined ret;
      };

      template < typename LR_ret >
      struct merge_binary_ret_< LR_ret, LR_ret >
      {
	typedef LR_ret ret;
      };

      template < typename L_ret >
      struct merge_binary_ret_< L_ret, undefined >
      {
	typedef L_ret ret;
      };

      template < typename R_ret >
      struct merge_binary_ret_< undefined, R_ret >
      {
	typedef R_ret ret;
      };

      template <>
      struct merge_binary_ret_< undefined, undefined >
      {
	typedef undefined ret;
      };

    } // end of namespace mln::trait::internal


    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_
    {
      // Construct a treillis in a static recursive way!

      typedef typename internal::set_binary_super_< Name,
						    typename Category_L<void>::super, L,
						    Category_R<void>, R >::ret
      L_ret;

      typedef typename internal::set_binary_super_< Name,
						    Category_L<void>, L,
						    typename Category_R<void>::super, R >::ret
      R_ret;

      typedef typename internal::merge_binary_ret_< L_ret, R_ret >::ret ret;
      // FIXME: Do we need to handle this search with a priority?
      // FIXME: for a result can be found in both branches... 
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
