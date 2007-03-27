// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_GEN_TRAITS_HH
# define OLN_CORE_GEN_TRAITS_HH

# include <oln/core/internal/category_of.hh>


// arith

# define oln_plus_trait(L, R) \
    typename oln::get_trait_<L, oln::plus_id, R>::ret

# define oln_minus_trait(L, R) \
    typename oln::get_trait_<L, oln::minus_id, R>::ret

# define oln_times_trait(L, R) \
    typename oln::get_trait_<L, oln::times_id, R>::ret

# define oln_div_trait(L, R) \
    typename oln::get_trait_<L, oln::div_id, R>::ret

# define oln_mod_trait(L, R) \
    typename oln::get_trait_<L, oln::mod_id, R>::ret

// cmp

# define oln_eq_trait(L, R) \
    typename oln::get_trait_<L, oln::eq_id, R>::ret

# define oln_neq_trait(L, R) \
    typename oln::get_trait_<L, oln::neq_id, R>::ret

// ord

# define oln_less_trait(L, R) \
    typename oln::get_trait_<L, oln::less_id, R>::ret

# define oln_leq_trait(L, R) \
    typename oln::get_trait_<L, oln::leq_id, R>::ret

# define oln_geq_trait(L, R) \
    typename oln::get_trait_<L, oln::geq_id, R>::ret

# define oln_greater_trait(L, R) \
    typename oln::get_trait_<L, oln::greater_id, R>::ret

// logic

# define oln_and_trait(L, R) \
    typename oln::get_trait_<L, oln::and_id, R>::ret

# define oln_or_trait(L, R) \
    typename oln::get_trait_<L, oln::or_id, R>::ret

# define oln_xor_trait(L, R) \
    typename oln::get_trait_<L, oln::xor_id, R>::ret








# define oln_internal_specialize_bin_trait_bool_(Name)	\
							\
  template <template <class> class Cl, typename L,	\
	    template <class> class Cr, typename R>	\
  struct set_trait_< Cl, L, Name##_id, Cr, R >		\
  {							\
    typedef bool ret;					\
  }


# define oln_internal_specialize_bin_trait_T_(Name)	\
							\
  template <template <class> class Cl, typename T,	\
	    template <class> class Cr>			\
  struct set_trait_< Cl, T, Name##_id, Cr, T >		\
  {							\
    typedef T ret;					\
  }




namespace oln
{
  
  // set_trait_

  template <template <class> class Cl, typename L,
	    typename Op,
	    template <class> class Cr, typename R>
  struct set_trait_;


  // ids

  struct plus_id;
  struct minus_id;
  struct times_id;
  struct div_id;
  struct mod_id;

  struct eq_id;
  struct neq_id;

  struct less_id;
  struct leq_id;
  struct geq_id;
  struct greater_id;

  struct and_id;
  struct or_id;
  struct xor_id;



  // default (see oln/core/concept/operators.hh)

  /*

  oln_internal_specialize_bin_trait_T_(plus);
  oln_internal_specialize_bin_trait_T_(minus);
  oln_internal_specialize_bin_trait_T_(times);
  oln_internal_specialize_bin_trait_T_(div);
  oln_internal_specialize_bin_trait_T_(mod);

  oln_internal_specialize_bin_trait_bool_(eq);
  oln_internal_specialize_bin_trait_bool_(neq);

  oln_internal_specialize_bin_trait_bool_(less);
  oln_internal_specialize_bin_trait_bool_(leq);
  oln_internal_specialize_bin_trait_bool_(geq);
  oln_internal_specialize_bin_trait_bool_(greater);

  oln_internal_specialize_bin_trait_T_(and);
  oln_internal_specialize_bin_trait_T_(or);
  oln_internal_specialize_bin_trait_T_(xor);

  */


  namespace internal
  {

    // get_trait_cat_
    
    template <typename Cl, typename L, typename Op, typename Cr, typename R>
    struct get_trait_cat__;
    
    template < template <class> class Cl, typename L,
	       typename Op,
	       template <class> class Cr, typename R >
    struct get_trait_cat__< stc::is<Cl>, L, Op, stc::is<Cr>, R >
    {
      typedef typename oln::set_trait_< Cl, L, Op, Cr, R >::ret ret;
    };
    
  } // end of namespace oln::internal


  template <typename L, typename Op, typename R>
  struct get_trait_ : public internal::get_trait_cat__< oln_category_of_(L), L, Op, oln_category_of_(R), R >
  {
  };


} // end of namespace oln


#endif // ! OLN_CORE_GEN_TRAITS_HH
