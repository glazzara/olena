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

/*!
 * \file   mln/trait/solve.hh
 *
 * \brief  FIXME
 *
 *
 */

# include <mln/core/category.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/ret.hh>


// FIXME: Just for the record (use it...)

#  ifndef MLN_DEBUG_TRAITS 
#  endif // ! MLN_DEBUG_TRAITS


# define mln_trait_unary(Name, T) typename mln::trait::solve_unary< Name, T >::ret

# define mln_trait_binary(Name, T1, T2) typename mln::trait::solve_binary< Name, T1, T2 >::ret



namespace mln
{

  namespace trait
  {

    /// Flag type for a not found trait.
    struct not_found {};

    /// Flag type for an undefined trait.
    struct undefined {};

    /// Flag type for a trait that is multiply undefined.
    struct multiply_defined {};



    // Utility meta-function: from a category (meta) and a type, get the super category.

    namespace internal
    {

      template < template <class> class Category, typename T,
		 typename Super_Category >
      struct helper_super_category_;

      template < template <class> class Category, typename T,
		 template <class> class Super_Category >
      struct helper_super_category_< Category, T,
				     Super_Category<void> >
      {
	typedef Super_Category<void> ret; // One super category: keep it.
      };

      template < template <class> class Category, typename T >
      struct helper_super_category_< Category, T,
				     void* > // Meaning: several super categories exist, depending on T.
      {
	typedef typename mln::category< T >::super ret; // Specific call depending on T.
      };


      template < template <class> class Category, typename T >
      struct super_category_ // Entry.
      {
	typedef typename helper_super_category_< Category, T,
						 typename Category<void>::super >::ret ret;
      };

    } // end of namespace mln::trait::internal



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

    template < template <class> class Name,
	       typename T >
    struct set_unary_< Name, Unknown, T > // Blocker; top of inheritance.
    {
      typedef not_found ret;
    };



    namespace internal
    {

      // Fwd decls.
      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct get_unary_;


      template < typename user_ret, /* != not_found and != undefined */
		 template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_get_unary_
      {
	typedef user_ret ret; // The user has defined 'ret' so we return it.
      };


      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_get_unary_< /* user_ret == */ not_found,
				Name, Category_T, T >
      {
	typedef not_found ret; // End of search due to a blocker; 'ret' is not found. 
      };


      template < template <class> class Name, typename Super_Category, typename T >
      struct helper_get_unary_rec_;

      template < template <class> class Name, template <class> class Super_Category_T, typename T >
      struct helper_get_unary_rec_< Name, Super_Category_T<void>, T > {
	typedef typename get_unary_<Name, Super_Category_T,       T>::ret ret;
      };

      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_get_unary_< /* user_ret == */ undefined,
				Name, Category_T, T >
      {
	typedef typename internal::super_category_< Category_T, T >::ret super;
	typedef typename helper_get_unary_rec_<Name, super, T>::ret ret; // No user ret definition => Recursion.
      };


      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct get_unary_
      {
	typedef typename mln::trait::set_unary_<Name, Category_T, T>::ret user_ret; // First get 'user_ret'
	typedef helper_get_unary_<user_ret, Name, Category_T, T> helper;            // Set the helper to make a decision.
	typedef mlc_ret(helper) ret;                                                // Return.
      };
    

      template < typename precise_ret,
		 template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_choose_unary_wrt_ /* precise_ret != undefined */
      {
	typedef precise_ret ret;                                   // -> A precise ret has been defined so it is it.
      };

      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_choose_unary_wrt_< /* precise_ret == */ undefined,
				       Name, Category_T, T >
      {
	typedef typename get_unary_<Name, Category_T, T>::ret ret; // -> Go up into the category inheritance
	                                                           //    to fetch a ret from 'set_unary_'s.
      };

      template < template <class> class Name,
		 template <class> class Category_T, typename T >
      struct helper_choose_unary_
      {
	typedef typename set_precise_unary_<Name, T>::ret precise_ret;
	typedef helper_choose_unary_wrt_< precise_ret, /* undefined or not (?) */
					  Name, Category_T, T> helper;
	typedef mlc_ret(helper) ret;
      };


      template < template <class> class Name,
		 typename Category_T_void, typename T >
      struct helper_solve_unary_; // This helper changes the category plain type into the category meta type.

      template < template <class> class Name,
		 template <class> class Category_T, typename void_T, typename T >
      struct helper_solve_unary_< Name, Category_T<void_T>, T >
	:
	helper_choose_unary_< Name, Category_T, T > // FIXME: typedef!
      {
	// FIXME: Check that void_T is 'void' or 'void*'. 
      };

      // FIXME: Remove below.

//       template < template <class> class Name,
// 		 template <class> class Category_T, typename T >
//       struct helper_solve_unary_< Name, Category_T<void*>, T >
// 	:
// 	helper_choose_unary_< Name, Category_T, T > // FIXME: typedef!
//       {};

    } // end of namespace mln::trait::internal


    // FIXME: Postfix solve_unary with a '-'(?)
    template < template <class> class Name,
	       typename T >
    struct solve_unary : internal::helper_solve_unary_< Name,
							typename mln::category<T>::ret, T > // FIXME: typedef!
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
    struct set_binary_
    {
      typedef undefined ret;
    };


    template < template <class, class> class Name,
	       typename L,
	       template <class> class Category_R, typename R >
    struct set_binary_< Name, Unknown, L, Category_R, R > // Left blocker.
    {
      typedef not_found ret;
    };

    template < template <class, class> class Name,
	       template <class> class Category_L, typename L,
	       typename R >
    struct set_binary_< Name, Category_L, L, Unknown, R > // Right blocker.
    {
      typedef not_found ret;
    };

    template < template <class, class> class Name,
	       typename L,
	       typename R >
    struct set_binary_< Name, Unknown, L, Unknown, R > // Blocker.
    {
      typedef not_found ret;
    };



    namespace internal
    {

      // triplet_ret_

      template < unsigned i_L_, unsigned i_R_, typename ret_ >
      struct triplet_
      {
	typedef ret_ ret;
      };


      // merge_triplets_

      template < typename L_trp, typename R_trp >
      struct merge_triplets_;

      template < unsigned L_i_L, unsigned L_i_R, typename L_ret,
		 unsigned R_i_L, unsigned R_i_R, typename R_ret >
      struct merge_triplets_< triplet_<L_i_L, L_i_R, L_ret>, 
			      triplet_<R_i_L, R_i_R, R_ret> >
      {
	typedef metal::bool_<(L_i_L <= R_i_L && L_i_R <= R_i_R)> take_L;
	typedef metal::bool_<(R_i_L <= L_i_L && R_i_R <= L_i_R)> take_R;
	typedef metal::or_<take_L, take_R> ok;
 	typedef typename metal::if_< metal::and_<ok, take_L>,
				     triplet_<L_i_L, L_i_R, L_ret>,
				     typename metal::if_< metal::and_<ok, take_R>,
							  triplet_<R_i_L, R_i_R, R_ret>,
							  triplet_<0,0, not_found> >::ret >::ret ret;
      };

      template < unsigned i_L, unsigned i_R, typename LR_ret >
      struct merge_triplets_< triplet_<i_L, i_R, LR_ret>, 
			      triplet_<i_L, i_R, LR_ret> >
      {
	typedef triplet_<i_L, i_R, LR_ret> ret;
      };


      template < unsigned L_i_L, unsigned L_i_R, unsigned L_i_max,
		 unsigned R_i_L, unsigned R_i_R, unsigned R_i_max >
      // L_i_max and R_i_max differ
      struct helper_merge_triplets_same_ret_
      {
	// The winning couple between L_* and R_* is the one which
	// maximum index is the smallest; for instance, with:
	//   left  branch giving L_i_L = 5 and L_i_R = 1 so L_i_max = 5
	//   right branch giving L_i_L = 3 and L_i_R = 4 so R_i_max = 4
	// the right branch wins.
	enum { i_L = (L_i_max < R_i_max ? L_i_L : R_i_L),
	       i_R = (L_i_max < R_i_max ? L_i_R : R_i_R) };
      };

      template < unsigned L_i_L, unsigned L_i_R, unsigned i_max,
		 unsigned R_i_L, unsigned R_i_R >
      // L_i_max is equal to R_i_max
      struct helper_merge_triplets_same_ret_< L_i_L, L_i_R, i_max,
					      R_i_L, R_i_R, i_max >
      {
	// The winning couple is the one with the minimum index.
	enum { L_i_min = (L_i_L < L_i_R ? L_i_L : L_i_R),
	       R_i_min = (R_i_L < R_i_R ? R_i_L : R_i_R),
	       i_L = (L_i_min < R_i_min ? L_i_L : R_i_L),
	       i_R = (L_i_min < R_i_min ? L_i_R : R_i_R) };
      };


      template < unsigned L_i_L, unsigned L_i_R, typename LR_ret,
		 unsigned R_i_L, unsigned R_i_R >
      struct merge_triplets_< triplet_<L_i_L, L_i_R, LR_ret>, 
			      triplet_<R_i_L, R_i_R, LR_ret> >
      {
	typedef helper_merge_triplets_same_ret_< L_i_L, L_i_R, (L_i_L > L_i_R ? L_i_L : L_i_R),
						 R_i_L, R_i_R, (R_i_L > R_i_R ? R_i_L : R_i_R) > helper;
	typedef triplet_<helper::i_L, helper::i_R, LR_ret> ret;
      };

      template < unsigned L_i_L, unsigned L_i_R, typename L_ret >
      struct merge_triplets_< triplet_<L_i_L, L_i_R, L_ret>,
			      triplet_<  0,     0,   not_found> >
      {
	typedef triplet_<L_i_L, L_i_R, L_ret> ret;
      };

      template < unsigned R_i_L, unsigned R_i_R, typename R_ret >
      struct merge_triplets_< triplet_<  0,     0,   not_found>,
			      triplet_<R_i_L, R_i_R, R_ret> >
      {
	typedef triplet_<R_i_L, R_i_R, R_ret> ret;
      };

      template <> // To disambiguate.
      struct merge_triplets_< triplet_<0, 0, not_found>,
			      triplet_<0, 0, not_found> >
      {
 	typedef triplet_<0u,0u, not_found> ret;
      };



      // Fwd decl.
      template < template <class, class> class Name,
		 unsigned i_L, template <class> class Category_L, typename L,
		 unsigned i_R, template <class> class Category_R, typename R >
      struct get_binary_;


      template < typename user_ret, /* != not_found and != undefined */
		 template <class, class> class Name,
		 unsigned i_L, template <class> class Category_L, typename L,
		 unsigned i_R, template <class> class Category_R, typename R >
      struct helper_get_binary_
      {
	typedef triplet_< i_L, i_R, user_ret > ret; // The user has defined 'ret' so we return it.
      };

      template < template <class, class> class Name,
		 unsigned i_L, template <class> class Category_L, typename L,
		 unsigned i_R, template <class> class Category_R, typename R >
      struct helper_get_binary_< /* user_ret == */ not_found,
				 Name,  i_L, Category_L, L,  i_R, Category_R, R >
      {
	typedef triplet_< 0, 0, not_found > ret; // End of search due to a blocker; 'ret' is not found. 
      };


      template < template <class, class> class Name,  // Nota bene: Seq means "super or equal".
		 unsigned i_L, typename Seq_Category_L, typename L,
		 unsigned i_R, typename Seq_Category_R, typename R >
      struct helper_get_binary_rec_;

      template < template <class, class> class Name,
		 unsigned i_L, template <class> class Seq_Category_L, typename L,
		 unsigned i_R, template <class> class Seq_Category_R, typename R >
      struct helper_get_binary_rec_< Name, i_L, Seq_Category_L<void>, L,  i_R, Seq_Category_R<void>, R >
      {
	typedef typename get_binary_<Name, i_L, Seq_Category_L, L,  i_R, Seq_Category_R, R>::ret ret;
      };


      template < template <class, class> class Name,
		 unsigned i_L, template <class> class Category_L, typename L,
		 unsigned i_R, template <class> class Category_R, typename R >
      struct helper_get_binary_< /* user_ret == */ undefined,
				 Name, i_L,Category_L, L, i_R,Category_R, R >
      {
	// No user definition for 'ret' so treillis construction in a static recursive way.

	// FIXME: We *do* need to handle this search with a priority!
	// FIXME: for a result can be found in both branches... 

	typedef typename super_category_< Category_L, L >::ret Super_Category_L;
	typedef typename super_category_< Category_R, R >::ret Super_Category_R;

	typedef helper_get_binary_rec_< Name,
					i_L + 1, Super_Category_L, L,
					i_R,     Category_R<void>, R > L_branch;
	typedef mlc_ret(L_branch) L_trp;

	typedef helper_get_binary_rec_< Name,
					i_L,     Category_L<void>, L,
					i_R + 1, Super_Category_R, R > R_branch;
	typedef mlc_ret(R_branch) R_trp;
	
	typedef typename merge_triplets_< L_trp, R_trp >::ret ret;
      };


      template < template <class, class> class Name,
		 unsigned i_L, template <class> class Category_L, typename L,
		 unsigned i_R, template <class> class Category_R, typename R >
      struct get_binary_
      {
	typedef typename mln::trait::set_binary_<Name, Category_L,L,
						       Category_R,R>::ret user_ret; // First get 'user_ret'
	typedef helper_get_binary_<user_ret, Name, i_L,Category_L,L,
				                   i_R,Category_R,R> helper;        // Set the helper to make a decision.
	typedef mlc_ret(helper) ret;                                                // Return a triplet.
      };


      template < typename precise_ret,
		 template <class, class> class Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct helper_choose_binary_wrt_ /* precise_ret != undefined */
      {
	typedef precise_ret ret;                                                 // -> A precise ret has been defined so it is it.
      };

      template < template <class, class> class Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct helper_choose_binary_wrt_< /* precise_ret == */ undefined,
					Name, Category_L, L, Category_R, R >
      {
	typedef typename get_binary_< Name,
				      0, Category_L, L,
				      0, Category_R, R >::ret triplet;           // Browse upwards the category inheritance
	typedef mlc_ret(triplet) ret;                                            // to fetch ret from 'get_binary_'s.
      };


      template < template <class, class> class Name,
		 template <class> class Category_L, typename L,
		 template <class> class Category_R, typename R >
      struct helper_choose_binary_
      {
	typedef typename set_precise_binary_<Name, L, R>::ret precise_ret; /* undefined or not (?) */
	typedef helper_choose_binary_wrt_<precise_ret, Name, Category_L,L, Category_R,R> helper;
	typedef mlc_ret(helper) ret;
      };


      template < template <class, class> class Name,
		 typename Category_L_void, typename L,
		 typename Category_R_void, typename R >
      struct helper_solve_binary_;

      template < template <class, class> class Name,
		 template <class> class Category_L, typename void_L, typename L,
		 template <class> class Category_R, typename void_R, typename R >
      struct helper_solve_binary_< Name,
				   Category_L<void_L>, L,
				   Category_R<void_R>, R > : helper_choose_binary_< Name,
										  Category_L, L,
										  Category_R, R >
      {
	// FIXME: Check that void_L and void_R are 'void' or 'void*'.
      };

    } // end of namespace mln::trait::internal


    // FIXME: Postfix solve_binary with a '-'(?)
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
