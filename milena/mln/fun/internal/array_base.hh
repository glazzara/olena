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

#ifndef MLN_FUN_INTERNAL_ARRAY_BASE_HH
# define MLN_FUN_INTERNAL_ARRAY_BASE_HH

/// \file mln/fun/internal/array_base.hh
///
/// Function mapping an Id i to a value v.

# include <vector>
# include <algorithm>
# include <mln/core/concept/function.hh>
# include <mln/util/array.hh>


namespace mln
{

  namespace fun
  {

    namespace internal
    {

      template <typename T>
      class array_base
      {
      public:

	typedef T result;
	typedef metal::true_ is_mutable;

	void resize(unsigned n);
	void resize(unsigned n, const T& val);
	unsigned size() const;

	const T& operator()(unsigned i) const;
	T& operator()(unsigned i);


      protected:

	std::vector<T> v_;

	/// Constructors
	/// \{

	/// Default.
	array_base();
	/// Constructs an array with \p nvalues.
	array_base(unsigned n);

	/// Constructs an array with \p nvalues and \p val as value.
	array_base(unsigned n, const T& val);

	/// Used in from_to(). Constructs that object from an util::array.
	/// Always prefer using from_to instead of this constructor.
	array_base(const util::array<T>& from);
	/// Used in from_to(). Constructs that object from an std::vector.
	/// Always prefer using from_to instead of this constructor.
	array_base(const std::vector<T>& from);

	/// \}

      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun



# ifndef MLN_INCLUDE_ONLY


  /// fun::internal::array_base

  namespace fun
  {

    namespace internal
    {

      template <typename T>
      inline
      array_base<T>::array_base()
      {
      }

      template <typename T>
      inline
      array_base<T>::array_base(unsigned n)
	: v_(n)
      {
      }

      template <typename T>
      inline
      array_base<T>::array_base(unsigned n, const T& val)
	: v_(n, val)
      {
      }

      template <typename T>
      inline
      array_base<T>::array_base(const util::array<T>& from)
	: v_(from.std_vector())
      {

      }

      template <typename T>
      inline
      array_base<T>::array_base(const std::vector<T>& from)
	: v_(from)
      {

      }

      template <typename T>
      inline
      void
      array_base<T>::resize(unsigned n)
      {
	v_.resize(n);
      }

      template <typename T>
      inline
      void
      array_base<T>::resize(unsigned n, const T& val)
      {
	v_.resize(n, val);
      }

      template <typename T>
      inline
      unsigned
      array_base<T>::size() const
      {
	return v_.size();
      }

      template <typename T>
      inline
      const T&
      array_base<T>::operator()(unsigned i) const
      {
	mln_precondition(i < v_.size());
	return v_[i];
      }

      template <typename T>
      inline
      T&
      array_base<T>::operator()(unsigned i)
      {
	mln_precondition(i < v_.size());
	return v_[i];
      }

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_ARRAY_BASE_HH
