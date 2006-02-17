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

#ifndef METALIC_CMP_HH
# define METALIC_CMP_HH

# include <mlc/bool.hh>
# include <mlc/is_a.hh>


/// Macros mlc_eq and mlc_neq.
# define mlc_eq( T1, T2) mlc::eq_ <T1, T2>
# define mlc_neq(T1, T2) mlc::neq_<T1, T2>


namespace mlc
{

  /// Check whether \a T is a mlc::abstract::value.
  template <typename T>
  struct is_value : public mlc_is_a(T, mlc::abstract::value)
  {
  };

  /// Check whether \a T is not a mlc::abstract::value.
  template <typename T>
  struct is_not_value : public not_<mlc_is_a(T, mlc::abstract::value)>
  {
  };



  /// Equality test between a couple of types.
  /// \{
  template <typename T1, typename T2>
  struct eq_ : private multiple_assert_< is_not_value<T1>,
					 is_not_value<T2> >,
	       public false_
  {
  };

  template <typename T>
  struct eq_ <T, T> : private assert_< is_not_value<T> >,
		      public true_
  {
  };
  /// \}

  /// Inequality test between a couple of types.
  /// \{
  template <typename T1, typename T2>
  struct neq_ : private multiple_assert_< is_not_value<T1>,
					  is_not_value<T2> >,
		public true_
  {
  };

  template <typename T>
  struct neq_ <T, T> : private assert_< is_not_value<T> >,
		       public false_
  {
  };
  /// \}


  /*--------------------------------------.
  | Syntactic sugar for flag comparison.  |
  `--------------------------------------*/

  /// Shortcuts for comparison with mlc::not_found.
  /// \{
  template <typename T>
  struct is_found : public neq_<T, not_found>
  {
  };

  template <typename T>
  struct is_not_found : public neq_<T, not_found>
  {
  };
  /// \}


  /// Check whether a type is a sound (supposedly before using it).
  template <typename T>
  struct is_ok : public and_list_< neq_<T, not_found>,
				   neq_<T, not_ok>,
				   neq_<T, undefined > >
  {
  };

} // end of namespace mlc


#endif // ! METALIC_CMP_HH
