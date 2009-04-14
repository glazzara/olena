// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

# include <vector>
# include <algorithm>
# include <mln/core/concept/function.hh>
# include <mln/util/array.hh>
# include <mln/metal/equal.hh>
# include <mln/tag/init.hh>


namespace mln
{

  /// Forward declaration.
  namespace fun {
    namespace i2v {
      template <typename T> class array;
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


      namespace internal
      {

        template <typename T, bool B = false >
        struct array_selector_
	 : public Function_i2v< i2v::array<T> >
	{
	};

	template <typename T>
	struct array_selector_<T,true>
	  : public Function_i2b< i2v::array<T> >
	{
	};

      } // end of namespace mln::fun::i2v::internal


      template <typename T>
      class array : public internal::array_selector_<T,mlc_equal(T,bool)::value>
      {
      public:

	/// Returned value types
	/// \{
	typedef T result;
	typedef typename std::vector<T>::reference mutable_result;

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

	to.reserve(from.nelements());
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

	to.reserve(from.nelements());
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


    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun


  template <typename T>
  inline
  fun::i2v::array<T> array(unsigned n, const T& t)
  {
    fun::i2v::array<T> tmp(n, t);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_FUN_I2V_ARRAY_HH
