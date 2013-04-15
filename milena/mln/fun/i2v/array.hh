// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_I2V_ARRAY_HH
# define MLN_FUN_I2V_ARRAY_HH

/// \file
///
/// Function mapping an Id i to a value v.

# include <vector>
# include <algorithm>
# include <mln/core/concept/function.hh>
# include <mln/util/array.hh>
# include <mln/metal/equal.hh>
# include <mln/tag/init.hh>

# include <mln/fun/internal/selector.hh>

namespace mln
{

  /// Forward declaration.
  namespace fun {
    namespace i2v {
      template <typename T> class array;
    } // end of namespace mln::fun::i2v
  } // end of namespace mln::fun

  namespace util {
    template <typename T> class array;
  }


  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      class array
	: public fun::internal::selector_from_result_<T, array<T> >::ret
      {
      public:

	/// Returned value types
	/// @{
	typedef T result;
	typedef typename std::vector<T>::reference mutable_result;
	/// @}

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


	/// Pre-allocate space.
	void reserve(unsigned n);

	/// Set the function size to \p n.
	void resize(unsigned n);
	/// Set the function size to \p n and initialize the value with
	/// \p val.
	void resize(unsigned n, const T& val);

	/// Append a new value in the function.
	void append(const T& val);

	/// Return the number of values.
	unsigned size() const;

	/// Const access to the ith value.
	result operator()(unsigned i) const;
	/// Read-Write access to the ith value.
	mutable_result operator()(unsigned i);

	/// Initialize an empty function.
	void init_(unsigned n);

	/// Return the underlying std::vector.
	const std::vector<T>& std_vector() const;

      protected:
	std::vector<T> v_;

      };


      /// Operator<<.
      template <typename T>
      std::ostream& operator<<(std::ostream& ostr,
			       const array<T>& a);


      /// \internal Conversion: fun::i2v::array -> util::array
      template <typename T1, typename T2>
      void
      from_to_(const fun::i2v::array<T1>& from, util::array<T2>& to);

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun

} // end of namespace mln


namespace std
{

  /// \internal Conversion: std::vectorfun::i2v::array<T> -> fun::i2v::array<T>
  template <typename T>
  inline
  void
  from_to_(const vector<T>& from, mln::fun::i2v::array<T>& to);

  /// \internal Conversion: std::vector<T> -> mln::fun::i2v::array<U>
  template <typename T, typename U>
  inline
  void
  from_to_(const vector<T>& from, mln::fun::i2v::array<U>& to);

} // end of namespace std


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  // Init.

  template <typename T1, typename T2>
  void init_(tag::function_t,
	     fun::i2v::array<T1>&	  f,
	     const fun::i2v::array<T2>&	  model)
  {
    f.init_(model.size());
  }


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
	: v_(n)
      {
      }

      template <typename T>
      inline
      array<T>::array(unsigned n, const T& val)
	: v_(n, val)
      {
      }

      template <typename T>
      inline
      array<T>::array(const util::array<T>& from)
	: v_(from.std_vector())
      {

      }

      template <typename T>
      inline
      array<T>::array(const std::vector<T>& from)
	: v_(from)
      {

      }

      template <typename T>
      inline
      void
      array<T>::reserve(unsigned n)
      {
	v_.reserve(n);
      }

      template <typename T>
      inline
      void
      array<T>::resize(unsigned n)
      {
	v_.resize(n);
      }

      template <typename T>
      inline
      void
      array<T>::append(const T& val)
      {
	v_.push_back(val);
      }

      template <typename T>
      inline
      void
      array<T>::resize(unsigned n, const T& val)
      {
	v_.resize(n, val);
      }

      template <typename T>
      inline
      unsigned
      array<T>::size() const
      {
	return v_.size();
      }

      template <typename T>
      inline
      typename array<T>::result
      array<T>::operator()(unsigned i) const
      {
	mln_precondition(i < v_.size());
	return v_[i];
      }

      template <typename T>
      inline
      typename array<T>::mutable_result
      array<T>::operator()(unsigned i)
      {
	mln_precondition(i < v_.size());
	return v_[i];
      }

      template <typename T>
      inline
      void
      array<T>::init_(unsigned n)
      {
	v_.resize(n);
      }

      template <typename T>
      inline
      const std::vector<T>&
      array<T>::std_vector() const
      {
	return v_;
      }


      // Operator <<.

      template <typename T>
      std::ostream& operator<<(std::ostream& ostr,
			       const array<T>& a)
      {
	ostr << '(';
	const unsigned n = a.size();
	for (unsigned i = 0; i < n; ++i)
	{
	  ostr << a(i);
	  if (i != n - 1)
	    ostr << ", ";
	}
	ostr << ')';
	return ostr;
      }


      // Conversion

      template <typename T1, typename T2>
      void
      from_to_(const array<T1>& from, util::array<T2>& to)
      {
        to.resize(from.size());

        for (unsigned i = 0; i < from.size(); ++i)
	  to[i] = convert::to<T2>(from(i));
      }

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun


  template <typename T>
  inline
  fun::i2v::array<T> array(unsigned n, const T& t)
  {
    fun::i2v::array<T> tmp(n, t);
    return tmp;
  }

} // end of namespace mln

namespace std
{

  template <typename T>
  inline
  void
  from_to_(const vector<T>& from, mln::fun::i2v::array<T>& to)
  {
    to = mln::fun::i2v::array<T>(from);
  }

  template <typename T, typename U>
  inline
  void
  from_to_(const vector<T>& from, mln::fun::i2v::array<U>& to)
  {
    to.resize(from.nelements());
    for (unsigned i = 0; i < from.size(); ++i)
      to(i) = mln::convert::to<U>(from[i]);
  }

} // end of namespace std

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_FUN_I2V_ARRAY_HH
