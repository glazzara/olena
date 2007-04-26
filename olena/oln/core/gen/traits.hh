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

# define oln_uminus_trait(T) \
    typename oln::get_utrait_<oln::uminus_id, T>::ret


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

# define oln_not_trait(T) \
    typename oln::get_utrait_<oln::not_id, T>::ret








# define oln_internal_specialize_bin_trait_bool_(Name)	\
							\
  template <typename L, typename R>			\
  struct set_trait_< Any, L, Name##_id, Any, R >	\
  {							\
    typedef bool ret;					\
  }


# define oln_internal_specialize_bin_trait_T_(Name)	\
							\
  template <typename T>					\
  struct set_trait_< Any, T, Name##_id, Any, T >	\
  {							\
    typedef T ret;					\
  }


# define oln_internal_specialize_un_trait_T_(Name)	\
							\
  template <typename T>					\
  struct set_utrait_< Name##_id, Any, T >		\
  {							\
    typedef T ret;					\
  }




namespace oln
{
  

  // set_trait_ for binary operators.

  template <template <class> class Cl, typename L,
	    typename Op,
	    template <class> class Cr, typename R>
  struct set_trait_;


  template <typename L, typename Op, typename R>
  struct set_trait_<Any, L, Op, Any, R>
  {
    // nothing => do not compile
  };


  template <template <class> class Cl, typename L,
	    typename Op,
	    template <class> class Cr, typename R>
  struct set_trait_
    :  // default is
    public set_trait_<Any, L, Op, Any, R>
  {
  };


  // "absorbing" a literal:
  template <typename L> struct Literal; // Fwd decl
  template <typename L, typename Op, template <class> class C, typename R>
  struct set_trait_<Literal, L, Op, C, R>
    : public set_trait_<C, R, Op, C, R>
  {};
  template <template <class> class C, typename L, typename Op, typename R>
  struct set_trait_<C, L, Op, Literal, R>
    : public set_trait_<C, L, Op, C, L>
  {};



  // set_utrait_ for unary operators.

  template <typename Op,
	    template <class> class C, typename T>
  struct set_utrait_;


  template <typename Op, typename T>
  struct set_utrait_<Op, Any, T>
  {
    // nothing => do not compile
  };


  template <typename Op,
	    template <class> class C, typename T>
  struct set_utrait_
    :  // default is
    public set_utrait_<Op, Any, T>
  {
  };



  // ids

  struct plus_id;
  struct minus_id;
  struct times_id;
  struct div_id;
  struct mod_id;

  struct uminus_id;

  struct plus_equal_id;
  struct minus_equal_id;
  struct modulus_equal_id;

  struct eq_id;
  struct neq_id;

  struct less_id;
  struct leq_id;
  struct geq_id;
  struct greater_id;

  struct and_id;
  struct or_id;
  struct xor_id;
  struct not_id;



  // default (see oln/core/concept/operators.hh)


  oln_internal_specialize_bin_trait_T_(plus);
  oln_internal_specialize_bin_trait_T_(minus);
  oln_internal_specialize_bin_trait_T_(times);
  oln_internal_specialize_bin_trait_T_(div);
  oln_internal_specialize_bin_trait_T_(mod);

  oln_internal_specialize_un_trait_T_(uminus);

  oln_internal_specialize_bin_trait_bool_(eq);
  oln_internal_specialize_bin_trait_bool_(neq);

  oln_internal_specialize_bin_trait_bool_(less);
  oln_internal_specialize_bin_trait_bool_(leq);
  oln_internal_specialize_bin_trait_bool_(geq);
  oln_internal_specialize_bin_trait_bool_(greater);

  oln_internal_specialize_bin_trait_T_(and);
  oln_internal_specialize_bin_trait_T_(or);
  oln_internal_specialize_bin_trait_T_(xor);

  oln_internal_specialize_un_trait_T_(not);


  // Fwd decl.
  template <typename T> struct value_;


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

    // get_utrait_cat_
    
    template <typename Op, typename C, typename T>
    struct get_utrait_cat__;
    
    template < typename Op,
	       template <class> class C, typename T >
    struct get_utrait_cat__< Op, stc::is<C>, T >
    {
      typedef typename oln::set_utrait_< Op, C, T >::ret ret;
    };

    // clean_for_trait_

    template <typename T>
    struct clean_for_trait_
    {
      typedef T ret; // default is identity
    };

    template <typename T>
    struct clean_for_trait_< T& >
    {
      // remove &
      typedef typename clean_for_trait_<T>::ret ret;
    }; 

    template <typename T>
    struct clean_for_trait_< const oln::value_<T>  >  { typedef T ret; };
    template <typename T>
    struct clean_for_trait_<       oln::value_<T>  >  { typedef T ret; };

    // builtins
    template <> struct clean_for_trait_< const unsigned >  { typedef unsigned ret; };
    template <> struct clean_for_trait_<       unsigned >  { typedef unsigned ret; };
    template <> struct clean_for_trait_< const int >       { typedef int      ret; };
    template <> struct clean_for_trait_<       int >       { typedef int      ret; };
    template <> struct clean_for_trait_< const float >     { typedef float    ret; };
    template <> struct clean_for_trait_<       float >     { typedef float    ret; };
    template <> struct clean_for_trait_< const double >    { typedef double   ret; };
    template <> struct clean_for_trait_<       double >    { typedef double   ret; };

  } // end of namespace oln::internal


  template <typename L, typename Op, typename R>
  struct get_trait_ : public internal::get_trait_cat__< oln_category_of_(L),
							typename internal::clean_for_trait_<L>::ret,
							Op,
							oln_category_of_(R),
							typename internal::clean_for_trait_<R>::ret >
  {
  };

  template <typename Op, typename T>
  struct get_utrait_ : public internal::get_utrait_cat__< Op,
							  oln_category_of_(T),
							  typename internal::clean_for_trait_<T>::ret >
  {
  };


} // end of namespace oln


// FIXME: Bad?
# include <oln/value/builtin.hh>


#endif // ! OLN_CORE_GEN_TRAITS_HH
