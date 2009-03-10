// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_I2V_ARRAY_HH
# define MLN_FUN_I2V_ARRAY_HH

/// \file mln/fun/i2v/array.hh
///
/// Function mapping an Id i to a value v.
///
/// \todo Change design so that there is no multiple inheritance:
///       array<T> : internal::array_base<T, E==array<T> > : Function_i2v<E>
///
/// \todo We should convert (with from_to) arrays that do not hold the
/// same element type, e.g., int_u8 -> int or vec<3,f> -> rgb8.

# include <vector>
# include <algorithm>
# include <mln/core/concept/function.hh>
# include <mln/fun/internal/array_base.hh>
# include <mln/util/array.hh>


namespace mln
{

  /// Forward declaration.
  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      class array;

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun



  namespace convert
  {

    namespace over_load
    {

      template <typename T>
      inline
      void
      from_to_(const util::array<T>& from, fun::i2v::array<T>& to);

      template <typename T, typename U>
      inline
      void
      from_to_(const util::array<T>& from, fun::i2v::array<U>& to);

      template <typename T>
      inline
      void
      from_to_(const std::vector<T>& from, fun::i2v::array<T>& to);

      template <typename T, typename U>
      inline
      void
      from_to_(const std::vector<T>& from, fun::i2v::array<U>& to);


    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert


  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      class array : public Function_i2v< array<T> >,
		    public internal::array_base<T>
      {
	typedef internal::array_base<T> super_base_;

      public:

	/// Mutable result type. The function results can be modified.
	typedef typename super_base_::mutable_result mutable_result;

	/// Constructors
	/// \{

	/// Default.
	array();
	/// Constructs a function with \p nvalues.
	array(unsigned n);
	/// Constructs a function with \p nvalues and \p val as default value.
	array(unsigned n, const T& val);

	/// Used in from_to(). Constructs that function from an util::array.
	/// Always prefer using from_to instead of this constructor.
	array(const util::array<T>& from);
	/// Used in from_to(). Constructs that function from an std::vector.
	/// Always prefer using from_to instead of this constructor.
	array(const std::vector<T>& from);
	/// \}

      };

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun



# ifndef MLN_INCLUDE_ONLY

  // Init.

  template <typename T1, typename T2>
  void init_(tag::function_t,
	     fun::i2v::array<T1>&	  f,
	     const fun::i2v::array<T2>&	  model)
  {
    f.init_(model.size());
  }


  // convert::from_to

  namespace convert
  {

    namespace over_load
    {

      template <typename T>
      inline
      void
      from_to_(const util::array<T>& from, fun::i2v::array<T>& to)
      {
	to = fun::i2v::array<T>(from);
      }

      template <typename T, typename U>
      inline
      void
      from_to_(const util::array<T>& from, fun::i2v::array<U>& to)
      {
	mlc_converts_to(T,U)::check();

	for (unsigned i = 0; i < from.nelements(); ++i)
	  to.append(convert::to<U>(from[i]));
      }

      template <typename T>
      inline
      void
      from_to_(const std::vector<T>& from, fun::i2v::array<T>& to)
      {
	to = fun::i2v::array<T>(from);
      }

      template <typename T, typename U>
      inline
      void
      from_to_(const std::vector<T>& from, fun::i2v::array<U>& to)
      {
	mlc_converts_to(T,U)::check();

	for (unsigned i = 0; i < from.size(); ++i)
	  to.append(convert::to<U>(from[i]));
      }


    } // end of namespace mln::convert::over_load

  } // end of namespace mln::convert



  /// fun::i2v::array

  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      inline
      array<T>::array()
      {
      }

      template <typename T>
      inline
      array<T>::array(unsigned n)
	: super_base_(n)
      {
      }

      template <typename T>
      inline
      array<T>::array(unsigned n, const T& val)
	: super_base_(n, val)
      {
      }

      template <typename T>
      inline
      array<T>::array(const util::array<T>& from)
	: super_base_(from)
      {

      }

      template <typename T>
      inline
      array<T>::array(const std::vector<T>& from)
	: super_base_(from)
      {

      }

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun

  template <typename T>
  inline
  fun::i2v::array<T> array(T t)
  {
    fun::i2v::array<T> tmp(t);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_I2V_ARRAY_HH
