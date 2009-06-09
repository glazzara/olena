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

#ifndef MLN_TRAIT_NEXT_SOLVE_BINARY_HH
# define MLN_TRAIT_NEXT_SOLVE_BINARY_HH

/// \file
///
///  FIXME

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
		  typename Category_L, typename L,
		  typename Category_R, typename R >
	struct trait_set_binary_;

	template < typename Name,
		  template <class> class Category_L, typename _l, typename L,
		  template <class> class Category_R, typename _r, typename R >
	struct trait_set_binary_< Name,
				Category_L<_l>, L,
				Category_R<_r>, R >
	{
	  typedef typename mln::trait::next::set_binary_<Name,
							 Category_L, L,
							 Category_R, R>::ret ret;
	};


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
	template < typename Name,
		  unsigned i_L, typename Category_L, typename L,
		  unsigned i_R, typename Category_R, typename R >
	struct get_binary_;


	template < typename user_ret, /* != not_found and != undefined */
		  typename Name,
		  unsigned i_L, typename Category_L, typename L,
		  unsigned i_R, typename Category_R, typename R >
	struct helper_get_binary_
	{
	  typedef triplet_< i_L, i_R, user_ret > ret; // The user has defined 'ret' so we return it.
	};

	template < typename Name,
		  unsigned i_L, typename Category_L, typename L,
		  unsigned i_R, typename Category_R, typename R >
	struct helper_get_binary_< /* user_ret == */ not_found,
				  Name,  i_L, Category_L, L,  i_R, Category_R, R >
	{
	  typedef triplet_< 0, 0, not_found > ret; // End of search due to a blocker; 'ret' is not found.
	};


	template < typename Name,
		  unsigned i_L, typename Category_L, typename L,
		  unsigned i_R, typename Category_R, typename R >
	struct helper_get_binary_< /* user_ret == */ undefined,
				  Name, i_L,Category_L, L, i_R,Category_R, R >
	{
	  // No user definition for 'ret' so treillis construction in a static recursive way.

	  // FIXME: We *do* need to handle this search with a priority!
	  // FIXME: for a result can be found in both branches...

	  typedef typename mln::internal::super_category_< Category_L, L >::ret Super_Category_L;
	  typedef typename mln::internal::super_category_< Category_R, R >::ret Super_Category_R;

	  typedef get_binary_< Name,
			      i_L + 1, Super_Category_L, L,
			      i_R,     Category_R,       R > L_branch;
	  typedef mlc_ret(L_branch) L_trp;

	  typedef get_binary_< Name,
			      i_L,     Category_L,       L,
			      i_R + 1, Super_Category_R, R > R_branch;
	  typedef mlc_ret(R_branch) R_trp;

	  typedef typename merge_triplets_< L_trp, R_trp >::ret ret;
	};


	template < typename Name,
		  unsigned i_L, typename Category_L, typename L,
		  unsigned i_R, typename Category_R, typename R >
	struct get_binary_
	{
	  typedef typename trait_set_binary_<Name, Category_L,L,
					    Category_R,R>::ret user_ret;  // First get 'user_ret'
	  typedef helper_get_binary_<user_ret, Name, i_L,Category_L,L,
						    i_R,Category_R,R> helper;        // Set the helper to make a decision.
	  typedef mlc_ret(helper) ret;                                                // Return a triplet.
	};


	template < typename precise_ret,
		  typename Name,
		  typename Category_L, typename L,
		  typename Category_R, typename R >
	struct helper_choose_binary_wrt_ /* precise_ret != undefined */
	{
	  typedef precise_ret ret;                                                 // -> A precise ret has been defined so it is it.
	};

	template < typename Name,
		  typename Category_L, typename L,
		  typename Category_R, typename R >
	struct helper_choose_binary_wrt_< /* precise_ret == */ undefined,
					  Name, Category_L, L, Category_R, R >
	{
	  typedef typename get_binary_< Name,
					0, Category_L, L,
					0, Category_R, R >::ret triplet;           // Browse upwards the category inheritance
	  typedef mlc_ret(triplet) ret;                                            // to fetch ret from 'get_binary_'s.
	};


	template < typename Name,
		  typename Category_L, typename L,
		  typename Category_R, typename R >
	struct helper_solve_binary_
	{
	  typedef typename set_precise_binary_<Name, L, R>::ret precise_ret; /* undefined or not (?) */
	  typedef helper_choose_binary_wrt_<precise_ret, Name, Category_L,L, Category_R,R> helper;
	  typedef mlc_ret(helper) ret;
	};

      } // end of namespace mln::trait::internal


      // FIXME: Postfix solve_binary with a '-'(?)
      template < typename Name,
		typename L_,
		typename R_ >
      struct solve_binary
      {
	typedef mln_exact(L_) L;
	typedef mln_exact(R_) R;
	typedef typename mln::category<L>::ret Category_L;
	typedef typename mln::category<R>::ret Category_R;
	typedef internal::helper_solve_binary_< Name, Category_L, L, Category_R, R > meta_code;
	typedef typename meta_code::ret ret;
      };

    } // end of namespace mln::trait::next

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_NEXT_SOLVE_BINARY_HH
