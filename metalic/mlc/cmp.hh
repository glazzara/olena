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

#ifndef MLC_CMP_HH
# define MLC_CMP_HH

# include <mlc/bexpr.hh>
# include <mlc/is_a.hh>
# include <mlc/logic.hh>
# include <mlc/assert.hh>



/// \{
/// Macros mlc_eq(T1, T2) and mlc_neq(T1, T2).

# define mlc_eq( T1, T2) mlc::eq_ < T1, T2 >
# define mlc_neq(T1, T2) mlc::neq_< T1, T2 >

/// \}



/// \{
/// Macros mlc_something(T) corresponding to mlc::something_<T>.

# define mlc_is_bexpr(T)     mlc::is_bexpr_< T >
# define mlc_is_not_bexpr(T) mlc::is_not_bexpr_< T >

# define mlc_is_found(T)     mlc::is_found_< T >
# define mlc_is_not_found(T) mlc::is_not_found_< T >

// FIXME: or (?) mlc_is_not_defined below
# define mlc_is_undefined(T) mlc::is_undefined_< T >
# define mlc_is_defined(T)   mlc::is_defined_< T >

# define mlc_is_ok(T)     mlc::is_ok_< T >
# define mlc_is_not_ok(T) mlc::is_not_ok_< T >

# define mlc_is_builtin(T)     mlc::is_builtin_< T >
# define mlc_is_not_builtin(T) mlc::is_not_builtin_< T >

# define mlc_is_const(T)     mlc::is_const_< T >
# define mlc_is_not_const(T)     mlc::is_not_const_< T >
# define mlc_is_reference(T) mlc::is_reference_< T >

/// \}



namespace mlc
{

  /*----------------------------------------------------------------.
  | Syntactic sugar for checking that a type is a value / a bexpr.  |
  `-----------------------------------------------------------------*/

  /// Check whether \a T is a mlc::abstract::value.
  template <typename T>
  struct is_value : public mlc_is_a(T, mlc::abstract::value)::bexpr
  {
  };

  /// Check whether \a T is not a mlc::abstract::value.
  template <typename T>
  struct is_not_value : public mlc_is_not_a(T, mlc::abstract::value)::bexpr
  {
  };


  /// Check whether \a T is a mlc::abstract::bexpr.
  template <typename T>
  struct is_bexpr : public mlc_is_a(T, mlc::abstract::bexpr)::bexpr
  {
  };

  /// Check whether \a T is not a mlc::abstract::bexpr.
  template <typename T>
  struct is_not_bexpr : public mlc_is_not_a(T, mlc::abstract::bexpr)::bexpr
  {
  };



  /*--------------------------.
  | Comparison between types  |
  `---------------------------*/

  /// Equality test between a couple of types.
  /// \{
  template <typename T1, typename T2>
  struct eq_ : private multiple_assert_< is_not_bexpr<T1>,
					 is_not_bexpr<T2> >,
	       public bexpr_<false>
  {
  };

  template <typename T>
  struct eq_ <T, T> : private assert_< is_not_bexpr<T> >,
		      public bexpr_<true>
  {
  };

  template <typename T1, typename T2, typename err = no_error_message>
  struct assert_equal_ : public assert_< eq_<T1, T2>, err >
  {
  };
  /// \}


  /// Inequality test between a couple of types.
  /// \{
  template <typename T1, typename T2>
  struct neq_ : private multiple_assert_< is_not_bexpr<T1>,
					  is_not_bexpr<T2> >,
		public bexpr_<true>
  {
  };

  template <typename T>
  struct neq_ <T, T> : private assert_< is_not_bexpr<T> >,
		       public bexpr_<false>
  {
  };
  /// \}



  /*--------------------------------------------------------------------------.
  | Syntactic sugar for checking the relationship between a type and a flag.  |
  `---------------------------------------------------------------------------*/

  /// Shortcuts for comparison with mlc::not_found.
  /// \{
  template <typename T>
  struct is_not_found_ : public mlc_is_a(T, mlc::not_found)::bexpr
  {
  };

  template <typename T>
  struct is_found_ : public mlc_is_not_a(T, mlc::not_found)::bexpr
  {
  };

  template <typename T, typename err = no_error_message>
  struct assert_found_ : public assert_< is_found_<T>, err >
  {
  };

  template <typename T, typename err = no_error_message>
  struct assert_not_found_ : public assert_< is_not_found_<T>, err >
  {
  };
  /// \}


  /// Shortcuts for testing inheritance from mlc::undefined.
  /// \{
  template <typename T>
  struct is_undefined_ : public mlc_is_a(T, mlc::undefined)::bexpr
  {
  };

  template <typename T>
  struct is_defined_ : public mlc_is_not_a(T, mlc::undefined)::bexpr
  {
  };

  template <typename T, typename err = no_error_message>
  struct assert_defined_ : public assert_< is_defined_<T>, err >
  {
  };
  /// \}


  /// Check whether a type is a sound (supposedly before using it).
  /// \{
  template <typename T>
  struct is_not_ok_ : public or_list_< is_not_found_<T>,
				       is_undefined_<T>,
				       mlc_is_a(T, mlc::not_ok) >::bexpr
  {
  };

  template <typename T>
  struct is_ok_ : public and_list_< is_found_<T>,
				    is_defined_<T>,
				    mlc_is_not_a(T, mlc::not_ok) >::bexpr
  {
  };

  template <typename T, typename err = no_error_message>
  struct assert_ok_ : public assert_< is_ok_<T>, err >
  {
  };
  /// \}


  /// Check whether a type is const.
  /// \{
  template <typename T>
  struct is_const_ : public bexpr_<false>
  {
  };

  template <typename T>
  struct is_const_< const T > : public bexpr_<true>
  {
  };

  template <typename T>
  struct is_not_const_ : public not_< is_const_<T> >::bexpr
  {
  };
  /// \}



  /// Check whether a type is a reference.
  /// \{
  template <typename T>
  struct is_reference_ : public bexpr_<false>
  {
  };

  template <typename T>
  struct is_reference_< T& > : public bexpr_<true>
  {
  };
  /// \}


  /// Check whether a type is a builtin type.
  /// \{

  template <typename T>
  struct is_builtin_ : public bexpr_<false>
  {
  };

  template <typename T> struct is_builtin_ < const T > : public is_builtin_< T >::bexpr {};
  template <typename T> struct is_builtin_ < T*      > : public is_builtin_< T >::bexpr {};
  template <typename T> struct is_builtin_ < T[]     > : public is_builtin_< T >::bexpr {};
  template <typename T,
            unsigned n> struct is_builtin_ < T[n]    > : public is_builtin_< T >::bexpr {};

  template <> struct is_builtin_ < void > : public bexpr_<true> {}; // FIXME: ?
  template <> struct is_builtin_ < bool > : public bexpr_<true> {};
  template <> struct is_builtin_ < char > : public bexpr_<true> {};

  template <> struct is_builtin_ <          float  > : public bexpr_<true> {};
  template <> struct is_builtin_ <          double > : public bexpr_<true> {};
  template <> struct is_builtin_ <     long double > : public bexpr_<true> {};

  template <> struct is_builtin_ < unsigned char   > : public bexpr_<true> {};
  template <> struct is_builtin_ <   signed char   > : public bexpr_<true> {};
  template <> struct is_builtin_ < unsigned short  > : public bexpr_<true> {};
  template <> struct is_builtin_ <   signed short  > : public bexpr_<true> {};
  template <> struct is_builtin_ < unsigned int    > : public bexpr_<true> {};
  template <> struct is_builtin_ <   signed int    > : public bexpr_<true> {};
  template <> struct is_builtin_ < unsigned long   > : public bexpr_<true> {};
  template <> struct is_builtin_ <   signed long   > : public bexpr_<true> {};

  template <typename T>
  struct is_not_builtin_ : public not_< is_builtin_<T> >::bexpr
  {
  };

  /// \}


} // end of namespace mlc


#endif // ! MLC_CMP_HH
