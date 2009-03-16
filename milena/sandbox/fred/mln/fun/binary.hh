// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_FUN_BINARY_HH
# define MLN_FUN_BINARY_HH

# include <mln/core/concept/meta_function.hh>
# include <mln/fun/spe/binary.hh>
# include <mln/trait/next/solve.hh>
# include <mln/trait/fun.hh>

namespace mln
{

  namespace fun
  {

    template <typename F>
    struct binary : mln::Meta_Function_vv2v< binary<F> >
    {

      template <typename T1, typename T2>
      struct with
      {
	typedef spe::binary<F, T1, T2> ret;
      };

      template <typename T1, typename T2>
      typename with<T1, T2>::ret::result operator()(const T1& a, const T2& b) const
      {
	typename with<T1, T2>::ret tmp;
	return tmp(a, b);
      }

    };

    template <typename F, typename P>
    struct binary_param: binary<F>
    {
      typedef P param;

      binary_param() {};
      binary_param(const param& p) : p_(p) {};

      void init(const param& p)
      {
	p_ = p;
      }

      param& parameter()
      {
	return p_;
      }

      protected:
	param p_;
    };

    template <typename F>
    struct binary_param<F, F>: binary<F>
    {
      typedef F param;

      void init(const param& p)
      {
	exact(*this) = p;
      }

      param& parameter()
      {
	return exact(*this);
      }
    };

  } // end of namespace mln::fun

} // end of namespace mln

#endif /* ! MLN_FUN_BINARY_HH */
