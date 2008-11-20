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

#ifndef MLN_CORE_VALUE_SHELL_HH
# define MLN_CORE_VALUE_SHELL_HH

# include <mln/core/concept/function.hh>
# include <mln/core/concept/image.hh>
# include <mln/value/internal/value_like.hh>

namespace mln
{

  namespace value
  {
    template <typename F, typename I>
    struct shell;
  }



  namespace trait
  {

    template <typename F, typename I>
    struct value_< mln::value::shell<F, I> >
      : value_< mln_result(F) >
    {
    };

  } // end of namespace trait

  namespace value
  {
    namespace impl
    {
      template <typename F, typename I, class C>
      struct shell_ { };

      template <typename F, typename I>
      struct shell_<F, I, Function_v2w2v<void> >
      {
	mln_value(I) set_ (I &ima, const mln_site(I) &s, const typename F::result &v);
      };

      template <typename F, typename I>
      struct shell_<F, I, Function_v2w_w2v<void> >
      {
	mln_value(I) set_ (I &ima, const mln_site(I) &s, const typename F::result &v);
      };
    }


    template <typename F, typename I>
    struct shell : public impl::shell_<F, I, typename F::category>, Value < shell<F, I> > // FIXME : should it be value_like ?
    {
      typedef typename F::result value;

      // Ctor
      shell(Image<I> &ima, const mln_site(I) &s);

      // Read
      operator value ();

      // Write
      value operator= (value);

      // Enc
      typedef void enc; // FIXME

      // Equiv
      typedef value equiv;

      // <<
      // std::ostream& operator<<(std::ostream& ostr);

      // >>
      // std::istream& operator>>(std::istream& istr);


    protected :
      I &ima;
      mln_site(I) s;
    };
  }

# ifndef MLN_INCLUDE_ONLY

  namespace value
  {

    // Ctor

    template <typename F, typename I>
    shell<F, I>::shell(Image<I> &ima, const mln_site(I) &s)
      : ima(exact(ima)), s(s)
    { }


    // Read for everyone
    template <typename F, typename I>
    shell<F, I>::operator value ()
    {
      return F()(ima(s));
    }

    // Write for everyone
    template <typename F, typename I>
    typename F::result shell<F, I>::operator= (typename F::result v)
    {
      set_(ima, s, v);
      return v;
    }

    //   template <typename F, typename I>
    //   std::ostream& shell<F, I>::operator<<(std::ostream& ostr)
    //   {
    //     ostr << ima(s);
    //     return ostr;
    //   }

    //   template <typename F, typename I>
    //   std::istream& shell<F, I>::operator>>(std::istream& istr)
    //   {
    //     ima(s) >> istr;
    //     return istr;
    //   }

    namespace impl
    {
      template <typename F, typename I>
      mln_value(I) shell_<F, I, Function_v2w2v<void> >::set_ (I &ima, const mln_site(I) &s, const typename F::result &v)
      {
	ima(s) = F().f_1(v);
	return ima(s);
      }

      template <typename F, typename I>
      mln_value(I) shell_<F, I, Function_v2w_w2v<void> >::set_ (I &ima, const mln_site(I) &s, const typename F::result &v)
      {
	ima(s) = F().f_1(ima(s), v);
	return ima(s);
      }

    }
  }

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

# endif // MLN_INCLUDE_ONLY

}; // end of namespace mln

#endif // MLN_CORE_VALUE_SHELL_HH
