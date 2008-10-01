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

/*! \file mln/fun/i2v/array.hh
 *
 * \brief FIXME.
 */

# include <vector>
# include <algorithm>
# include <mln/core/concept/function.hh>


namespace mln
{

  namespace fun
  {

    namespace i2v
    {

      template <typename T>
      class array : public Function_i2v< array<T> >
      {
      public:

	typedef T result;

	array();
	array(unsigned n);

	void resize(unsigned n);
	unsigned size() const;

	T operator()(unsigned i) const;
	T& operator()(unsigned i);
	
      private:
	std::vector<T> v_;
      };

    } // end of namespace mln::fun::i2v

  } // end of namespace mln::fun


# ifndef MLN_INCLUDE_ONLY

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
      T
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
