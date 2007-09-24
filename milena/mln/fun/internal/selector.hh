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

#ifndef MLN_FUN_INTERNAL_SELECTOR_HH
# define MLN_FUN_INTERNAL_SELECTOR_HH

/*! \file mln/fun/internal/selector.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/core/concept/point_site.hh>
# include <mln/metal/unqualif.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/vec.hh>


namespace mln
{

  namespace fun
  {

    namespace internal
    {

      //       Function_v2v
      //         |
      //         + ---------------------- Function_v2b
      //         |                           |
      //         + -- Function_i2v           |
      //         |                           |
      //         + -- Function_p2v           |
      //                |                    |
      //                + -- Function_p2b -- +
      //                |
      //                + -- Function_p2p

      enum
	{
	  b_,
	  i_,
	  p_,
	  v_,
	  x_
	};

      template <int arg, int res, typename E> struct helper_selector_;

      // b2* => v2v type, except for v2b
      template <typename E>
      struct helper_selector_< b_, b_, E > { typedef Function_v2b<E> ret; };
      template <typename E>
      struct helper_selector_< b_, i_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< b_, p_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< b_, v_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< b_, x_, E > { typedef Function_v2v<E> ret; };

      // i2* => i2v type
      template <typename E>
      struct helper_selector_< i_, b_, E > { typedef Function_i2v<E> ret; };
      template <typename E>
      struct helper_selector_< i_, i_, E > { typedef Function_i2v<E> ret; };
      template <typename E>
      struct helper_selector_< i_, p_, E > { typedef Function_i2v<E> ret; };
      template <typename E>
      struct helper_selector_< i_, v_, E > { typedef Function_i2v<E> ret; };
      template <typename E>
      struct helper_selector_< i_, x_, E > { typedef Function_i2v<E> ret; };

      // p2*
      template <typename E>
      struct helper_selector_< p_, b_, E > { typedef Function_p2b<E> ret; };
      template <typename E>
      struct helper_selector_< p_, i_, E > { typedef Function_p2v<E> ret; }; // no p2i type => p2v
      template <typename E>
      struct helper_selector_< p_, p_, E > { typedef Function_p2p<E> ret; };
      template <typename E>
      struct helper_selector_< p_, v_, E > { typedef Function_p2v<E> ret; };
      template <typename E>
      struct helper_selector_< p_, x_, E > { typedef Function_p2v<E> ret; };

      // v2* => v2v type, except for v2b
      template <typename E>
      struct helper_selector_< v_, b_, E > { typedef Function_v2b<E> ret; };
      template <typename E>
      struct helper_selector_< v_, i_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< v_, p_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< v_, v_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< v_, x_, E > { typedef Function_v2v<E> ret; };

      // x2* => v2v type
      template <typename E>
      struct helper_selector_< x_, b_, E > { typedef Function_v2b<E> ret; };
      template <typename E>
      struct helper_selector_< x_, i_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< x_, p_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< x_, v_, E > { typedef Function_v2v<E> ret; };
      template <typename E>
      struct helper_selector_< x_, x_, E > { typedef Function_v2v<E> ret; };

      // tag_

      template <typename T> struct tag_;

      template <>
      struct tag_< bool >
      {
	enum { value = b_ };
      };

      template <>
      struct tag_< unsigned >
      {
	enum { value = i_ };
      };

      template <unsigned n, typename T>
      struct tag_< metal::vec<n,T> >
      {
	enum { value = x_ };
      };

      template <typename T>
      struct tag_
      {
	enum { value = mlc_is_a(T, Point_Site)::to_bool
	       ? p_
	       : v_ };
      };


      template <typename R_, typename A_, typename E>
      struct selector_
      {
	typedef mlc_unqualif(R_) R;
	typedef mlc_unqualif(A_) A;
	enum { arg = tag_<A>::value,
	       res = tag_<R>::value };
	typedef typename helper_selector_<arg, res, E>::ret ret;
      private:
	selector_();
      };


      template <typename R_, typename E>
      struct selector_p2_
      {
	typedef mlc_unqualif(R_) R;
	enum { res = tag_<R>::value };
	typedef typename helper_selector_<p_, res, E>::ret ret;
      private:
	selector_p2_();
      };

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_SELECTOR_HH
