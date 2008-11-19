// Copyright (C) 2008 EPITA Research and Development Laboratory
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

    template <typename T>
    inline
    void
    from_to(const util::array<T>& from, fun::i2v::array<T>& to);

    template <typename T>
    inline
    void
    from_to(const std::vector<T>& from, fun::i2v::array<T>& to);

  } // end of namespace mln::convert



  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      class array : public Function_i2v< array<T> >
      {
      public:

	typedef T result;

	/// Constructors
	/// \{

	/// Default.
	array();
	/// Constructs a function with \p nvalues.
	array(unsigned n);

	/// Used in from_to(). Constructs that function from an util::array.
	/// Always prefer using from_to instead of this constructor.
	array(const util::array<T>& from);
	/// Used in from_to(). Constructs that function from an std::vector.
	/// Always prefer using from_to instead of this constructor.
	array(const std::vector<T>& from);

	/// \}

	void resize(unsigned n);
	unsigned size() const;

	const T& operator()(unsigned i) const;
	T& operator()(unsigned i);

      private:
	std::vector<T> v_;
      };

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun


# ifndef MLN_INCLUDE_ONLY


  // convert::from_to

  namespace convert
  {

    template <typename T>
    inline
    void
    from_to(const util::array<T>& from, fun::i2v::array<T>& to)
    {
      to = fun::i2v::array<T>(from);
    }

    template <typename T>
    inline
    void
    from_to(const std::vector<T>& from, fun::i2v::array<T>& to)
    {
      to = fun::i2v::array<T>(from);
    }

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
      {
	resize(n);
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
      array<T>::resize(unsigned n)
      {
	v_.resize(n);
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
      const T&
      array<T>::operator()(unsigned i) const
      {
	mln_precondition(i < v_.size());
	return v_[i];
      }

      template <typename T>
      inline
      T&
      array<T>::operator()(unsigned i)
      {
	mln_precondition(i < v_.size());
	return v_[i];
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
