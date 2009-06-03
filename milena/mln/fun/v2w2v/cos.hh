// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_FUN_V2W2V_COS_HH
# define MLN_FUN_V2W2V_COS_HH

/*! \file mln/fun/v2w2v/cos.hh
 *
 *  \brief Cos functor.
 *
 *  \see mln/math/.
 */

# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>

# include <mln/math/cos.hh>
# include <mln/math/acos.hh>


namespace mln
{

  namespace fun
  {

    namespace v2w2v
    {

      /*! \brief Cosinus bijective functor.
       *
       *  \c V is the type of input values and the result type.
       *
       *  \see mln::math::cos.
       */
      template <typename V>
      struct cos : public Function_v2v< cos<V> >
      {
	typedef V result;
	V operator()(const V& v) const;
	V f_1       (const V& v) const;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      V
      cos<V>::operator()(const V& v) const
      {
	return mln::math::cos (v);
      }

      template <typename V>
      inline
      V
      cos<V>::f_1(const V& v) const
      {
	return mln::math::acos (v);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2w2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2W2V_COS_HH
