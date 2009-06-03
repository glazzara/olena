// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CORE_VALUE_MIXIN_HH
# define MLN_CORE_VALUE_MIXIN_HH

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/value/internal/value_like.hh>

namespace mln
{

  namespace value
  {
    template <typename T, typename F>
    struct mixin;
  }

  namespace trait
  {

    template <typename T, typename F>
    struct value_< mln::value::mixin<T, F> >
      : value_< T > // FIXME
    {
      static const T max()
      {
	static const F f = F();
	return f.max();
      }
    };

  } // end of namespace trait


  namespace value
  {

    template <typename T, typename F>
    struct mixin : T // , Value < mixin<T, F> >, internal::value_like_ <T, mln_enc(T), mixin<T, F>, mixin<T, F> >
    {
      // typedef T value;

      mixin()
      {
      }

      mixin(const T& t)
	: T(t)
      {
      }

      mixin(const mixin& rhs)
	: T(rhs)
      {
      }

      mixin& operator=(const mixin& rhs)
      {
	// FIXME: (?) Test if (this == &rhs)...
	this->T::operator=(rhs);
	return *this;
      }

      mixin& operator=(const T& t)
      {
	// FIXME: (?) Test if (this == &t)...
	this->T::operator=(t);
	return *this;
      }
    };

  } // end of namespace mln::value


  template <typename T, typename F>
  bool operator < (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  {
    static F f;
    return f.less(lhs, rhs);
  }

  // FIXME : check if there is an operator== in F

  //  template <typename T, typename F>
  //  bool operator == (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  //  {
  //    static F f;
  //    return f.equal(lhs, rhs);
  //  }

  template <typename T, typename F>
  bool operator == (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  {
    static F f;
    return !(f.less(lhs, rhs) || f.less(rhs, lhs));
  }

  template <typename T, typename F>
  bool operator <= (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  {
    static F f;
    return f.less(lhs, rhs) || lhs == rhs;
  }

  template <typename T, typename F>
  bool operator > (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  {
    static F f;
    return ! (f.less(lhs, rhs) || lhs == rhs);
  }

  template <typename T, typename F>
  bool operator >= (const value::mixin<T,F>& lhs, const value::mixin<T,F>& rhs)
  {
    static F f;
    return ! f.less(lhs, rhs);
  }

# ifndef MLN_INCLUDE_ONLY

# endif // MLN_INCLUDE_ONLY

}; // end of namespace mln

#endif // MLN_CORE_VALUE_MIXIN_HH
