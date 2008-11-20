// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_VALUE_MIXIN_HH
# define MLN_CORE_VALUE_MIXIN_HH

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/op_less.hh> // FIXME : include all modificators

namespace mln
{
  namespace value
  {
    template <typename T, template <typename> class F>
    struct mixin;
  }

  namespace trait
  {

    template <typename T, template <typename> class F>
    struct value_< mln::value::mixin<T, F> >
      : value_< T > // FIXME
    {
    };

  } // end of namespace trait

  namespace value
  {

    template <typename T, template <typename> class F>
    struct mixin : T, Value < mixin<T, F> >, internal::value_like_ <T, mln_enc(T), mixin<T, F>, mixin<T, F> >
    {
      typedef T value;

      // Ctor ?

      // operator (T) ();

    };
  }

# ifndef MLN_INCLUDE_ONLY

  namespace value
  {

    template <typename T>
    struct mixin <T, op_less> : T, Value < mixin<T, op_less> >, internal::value_like_ <T, mln_enc(T), mixin<T, op_less>, mixin<T, op_less> >
    {
      typedef mixin<T, op_less> exact;

      bool operator< (const exact &rhs)
      {
	static op_less<T> l;
	return l((T)*this, (T)rhs);
      }
    };

    // Ctor

    // template <typename T, template <typename> class F>
    // mixin<T, F>::operator(T)()
    // {
    //   return (T)*this;
    // }

  }

/*
  template <typename F, typename I>
  std::ostream& operator<<(std::ostream& ostr, value::shell<F, I> &s)
  {
    ostr << (typename value::shell<F, I>::value) s;
    return ostr;
  }

  template <typename F, typename I>
  std::istream& operator>>(std::istream& istr, value::shell<F, I> &s)
  {
    (typename value::shell<F, I>::value) s >> istr;
    return istr;
  }
*/

# endif // MLN_INCLUDE_ONLY

}; // end of namespace mln

#endif // MLN_CORE_VALUE_MIXIN_HH
