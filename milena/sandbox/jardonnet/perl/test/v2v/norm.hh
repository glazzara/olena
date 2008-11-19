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

#ifndef MLN_FUN_V2V_NORM_HH
# define MLN_FUN_V2V_NORM_HH

/*! \file mln/fun/v2v/norm.hh
 *
 *  \brief Norm functors.
 *
 *  \see mln/norm/.
 */

// FIXME: Move to mln/fun/x2v/?

# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>

# include <mln/norm/all.hh>


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /*! \brief L1-norm.
       *
       *  \c V is the type of input values; \c R is the result type.
       *
       *  \see mln::norm::l1.
       */
      template <typename V, typename R>
      struct l1_norm : public Function_v2v< l1_norm<V, R> >
      {
	typedef R result;
	R operator()(const V& v) const;
      };

      /*! \brief L2-norm.
       *
       *  \c V is the type of input values; \c R is the result type.
       *
       *  \see mln::norm::l2.
       */
      template <typename V, typename R>
      struct l2_norm : public Function_v2v< l2_norm<V, R> >
      {
	typedef R result;
	R operator()(const V& v) const;
      };

      /*! \brief L-infty norm.
       *
       *  \c V is the type of input values; \c R is the result type.
       *
       *  \see mln::norm::linfty.
       */
      template <typename V, typename R>
      struct linfty_norm : public Function_v2v< linfty_norm<V, R> >
      {
	typedef R result;
	R operator()(const V& v) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename R>
      inline
      R
      l1_norm<V, R>::operator()(const V& v) const
      {
	return mln::norm::l1 (v);
      }

      template <typename V, typename R>
      inline
      R
      l2_norm<V, R>::operator()(const V& v) const
      {
	return mln::norm::l2 (v);
      }

      template <typename V, typename R>
      inline
      R
      linfty_norm<V, R>::operator()(const V& v) const
      {
	return mln::norm::linfty (v);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_NORM_HH
