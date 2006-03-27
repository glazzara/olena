// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef METALIC_LOGIC_HH
# define METALIC_LOGIC_HH

# include <mlc/bexpr.hh>
# include <mlc/assert.hh>
# include <mlc/is_a.hh>




/// Macros corresponding to binary logic Boolean expressions.
/// \{

# define  mlc_and(L, R)   mlc::and_ < L, R >
# define  mlc_nand(L, R)  mlc::nand_< L, R >
# define  mlc_or(L, R)    mlc::or_  < L, R >
# define  mlc_nor(L, R)   mlc::nor_ < L, R >
# define  mlc_xor(L, R)   mlc::xor_ < L, R >
# define  mlc_xnor(L, R)  mlc::xnor_< L, R >

/// \}




namespace mlc
{

  namespace ERROR
  {

    struct PARAMETER_OF_mlc_not_SHOULD_BE_A_BEXPR;

    struct FIRST_PARAMETER_OF_mlc_and_SHOULD_BE_A_BEXPR;
    struct FIRST_PARAMETER_OF_mlc_nand_SHOULD_BE_A_BEXPR;
    struct FIRST_PARAMETER_OF_mlc_or_SHOULD_BE_A_BEXPR;
    struct FIRST_PARAMETER_OF_mlc_nor_SHOULD_BE_A_BEXPR;
    struct FIRST_PARAMETER_OF_mlc_xor_SHOULD_BE_A_BEXPR;
    struct FIRST_PARAMETER_OF_mlc_xnor_SHOULD_BE_A_BEXPR;

    struct SECOND_PARAMETER_OF_mlc_and_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_nand_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_or_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_nor_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_xor_SHOULD_BE_A_BEXPR;
    struct SECOND_PARAMETER_OF_mlc_xnor_SHOULD_BE_A_BEXPR;

  } // end of mlc::ERROR



  /*! \class mlc::not_<T>
  **
  ** Logical unary 'not' operator on a Boolean expression type.  This
  ** class is also a Boolean expression type.
  */

  template <typename T>
  struct not_

    : private assert_< mlc_is_a(T, mlc::abstract::bexpr),
		       mlc::ERROR::PARAMETER_OF_mlc_not_SHOULD_BE_A_BEXPR >,

      public bexpr_<( !mlc_bool(T) )>
  {};



  /*! \class mlc::and_<L,R>
  **
  ** Logical binary 'and' operator on a couple of Boolean expression
  ** types.  This class is also a Boolean expression type.
  **
  ** \see mlc::and_list_<..>
  */

  template <typename L, typename R>
  struct and_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_and_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_and_SHOULD_BE_A_BEXPR >,

      public bexpr_<( mlc_bool(L) && mlc_bool(R) )>
  {};



  /*! \class mlc::nand_<L,R>
  **
  ** Logical binary 'not and' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Design note: an equivalent is mlc::not_< mlc::and_<L,R> >.
  */

  template <typename L, typename R>
  struct nand_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_nand_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_nand_SHOULD_BE_A_BEXPR >,

      public bexpr_<( !(mlc_bool(L) && mlc_bool(R)) )>
  {}; 



  /*! \class mlc::or_<L,R>
  **
  ** Logical binary 'or' operator on a couple of Boolean expression
  ** types.  This class is also a Boolean expression type.
  **
  ** \see mlc::or_list_<..>
  */

  template <typename L, typename R>
  struct or_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_or_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_or_SHOULD_BE_A_BEXPR >,
      
      public bexpr_<( mlc_bool(L) || mlc_bool(R) )>
  {};



  /*! \class mlc::nor_<L,R>
  **
  ** Logical binary 'not or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Design note: an equivalent is mlc::not_< mlc::or_<L,R> >.
  */

  template <typename L, typename R>
  struct nor_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_nor_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_nor_SHOULD_BE_A_BEXPR >,
    
      public bexpr_<( !(mlc_bool(L) || mlc_bool(R)) )>
  {};



  /*! \class mlc::xor_<L,R>
  **
  ** Logical binary 'exclusive or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  */

  template <typename L, typename R>
  struct xor_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_xor_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_xor_SHOULD_BE_A_BEXPR >,

      public bexpr_<( mlc_bool(L) != mlc_bool(R) )>
  {};


  /*! \class mlc::xnor_<L,R>
  **
  ** Logical binary 'exclusive not or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  */

  template <typename L, typename R>
  struct xnor_

    : private assert_< mlc_is_a(L, mlc::abstract::bexpr),
		       mlc::ERROR::FIRST_PARAMETER_OF_mlc_xnor_SHOULD_BE_A_BEXPR >,

      private assert_< mlc_is_a(R, mlc::abstract::bexpr),
		       mlc::ERROR::SECOND_PARAMETER_OF_mlc_xnor_SHOULD_BE_A_BEXPR >,

      public bexpr_<( !(mlc_bool(L) != mlc_bool(R)) )>
  {};
  



  /// Internal helpers for logical operators between several Boolean types

  namespace internal
  {
    // FIXME: doc

    template <typename A>
    struct is_bexpr_or_none_
      : public mlc_is_a(A, mlc::abstract::bexpr)
    {
    };

    template <>
    struct is_bexpr_or_none_ < none >
      : public bexpr_<true>
    {
    };

    // va_bexpr_

    template <typename A>
    struct va_bexpr_
    {
      typedef typename A::bexpr ret;
    };

    template <>
    struct va_bexpr_ <none>
    {
      typedef none ret;
    };


    // or_list_2_

    template <typename A1, typename A2>
    struct or_list_2_
      : public or_<A1, A2>
    {};

    template <>
    struct or_list_2_ <none, none>
      : public bexpr_<false>
    {};

    template <typename A1>
    struct or_list_2_ <A1, none>
      : public A1::bexpr
    {};

    template <typename A2>
    struct or_list_2_ <none, A2>; // forbidden

    // or_list_4_

    template <typename A1, typename A2, typename A3, typename A4>
    struct or_list_4_
      : public or_list_2_< typename or_list_2_<A1, A2>::bexpr,
			   typename or_list_2_<A3, A4>::bexpr >::bexpr
    {};

    template <>
    struct or_list_4_ <none, none, none, none>
      : public bexpr_<false>
    {};

    // or_list_

    template <typename A1, typename A2, typename A3, typename A4,
	      typename A5, typename A6, typename A7, typename A8>
    struct or_list_
      : public or_list_2_< typename or_list_4_<A1, A2, A3, A4>::bexpr,
			   typename or_list_4_<A5, A6, A7, A8>::bexpr >::bexpr
    {};


    // and_list_2_

    template <typename A1, typename A2>
    struct and_list_2_
      : public and_<A1, A2>
    {};

    template <>
    struct and_list_2_ <none, none>
      : public bexpr_<true>
    {};

    template <typename A1>
    struct and_list_2_ <A1, none>
      : public A1
    {};

    template <typename A2>
    struct and_list_2_ <none, A2>; // forbidden

    // and_list_4_

    template <typename A1, typename A2, typename A3, typename A4>
    struct and_list_4_
      : public and_list_2_< typename and_list_2_<A1, A2>::bexpr,
			    typename and_list_2_<A3, A4>::bexpr >::bexpr
    {};

    template <>
    struct and_list_4_ <none, none, none, none>
      : public bexpr_<true>
    {};

    // and_list_

    template <typename A1, typename A2, typename A3, typename A4,
	      typename A5, typename A6, typename A7, typename A8>
    struct and_list_
      : public and_list_2_< typename and_list_4_<A1, A2, A3, A4>::bexpr,
			    typename and_list_4_<A5, A6, A7, A8>::bexpr >::bexpr
    {};

  } // end of mlc::internal



  /*! \class mlc::or_list_<..>
  **
  ** Logical n-ary 'or' operator on a set of Boolean expression types.
  ** The number of arguments (parameters) should be at least 3 and at
  ** most 8.  This class is also a Boolean expression type.
  **
  ** Sample use:
  ** mlc::or_list_< mlc::eq_<T, int>,
  **                mlc_is_a(T, mlc::int_),
  **                mlc_is_a(T, my::integer) >
  **
  ** \see mlc::or_<L,R> mlc::and_list_<..>
  */

  template <typename A1,
	    typename A2,
	    typename A3,
	    typename A4 = none,
	    typename A5 = none,
	    typename A6 = none,
	    typename A7 = none,
	    typename A8 = none>
  struct or_list_ :

    private multiple_assert_< internal::is_bexpr_or_none_<A1>,
			      internal::is_bexpr_or_none_<A2>,
			      internal::is_bexpr_or_none_<A3>,
			      internal::is_bexpr_or_none_<A4>,
			      internal::is_bexpr_or_none_<A5>,
			      internal::is_bexpr_or_none_<A6>,
			      internal::is_bexpr_or_none_<A7>,
			      internal::is_bexpr_or_none_<A8> >,

    public internal::or_list_< typename internal::va_bexpr_<A1>::ret,
			       typename internal::va_bexpr_<A2>::ret,
			       typename internal::va_bexpr_<A3>::ret,
			       typename internal::va_bexpr_<A4>::ret,
			       typename internal::va_bexpr_<A5>::ret,
			       typename internal::va_bexpr_<A6>::ret,
			       typename internal::va_bexpr_<A7>::ret,
			       typename internal::va_bexpr_<A8>::ret >
  {
  };



  /*! \class mlc::and_list_<..>
  **
  ** Logical n-ary 'and' operator on a set of Boolean expression types.
  ** The number of arguments (parameters) should be at least 3 and at
  ** most 8.  This class is also a Boolean expression type.
  **
  ** \see mlc::and_<L,R> mlc::or_list_<..>
  */

  template <typename A1,
	    typename A2,
	    typename A3,
	    typename A4 = none,
	    typename A5 = none,
	    typename A6 = none,
	    typename A7 = none,
	    typename A8 = none>
  struct and_list_ :

    private multiple_assert_< internal::is_bexpr_or_none_<A1>,
			      internal::is_bexpr_or_none_<A2>,
			      internal::is_bexpr_or_none_<A3>,
			      internal::is_bexpr_or_none_<A4>,
			      internal::is_bexpr_or_none_<A5>,
			      internal::is_bexpr_or_none_<A6>,
			      internal::is_bexpr_or_none_<A7>,
			      internal::is_bexpr_or_none_<A8> >,

    public internal::and_list_< typename internal::va_bexpr_<A1>::ret,
				typename internal::va_bexpr_<A2>::ret,
				typename internal::va_bexpr_<A3>::ret,
				typename internal::va_bexpr_<A4>::ret,
				typename internal::va_bexpr_<A5>::ret,
				typename internal::va_bexpr_<A6>::ret,
				typename internal::va_bexpr_<A7>::ret,
				typename internal::va_bexpr_<A8>::ret >
  {
  };


} // end of namespace mlc


#endif // ! METALIC_LOGIC_HH
