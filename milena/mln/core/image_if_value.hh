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

#ifndef MLN_CORE_IMAGE_IF_VALUE_HH
# define MLN_CORE_IMAGE_IF_VALUE_HH

/*! \file mln/core/image_if_value.hh
 *
 * \brief Definition of a image_if_value image.
 */

# include <mln/core/internal/image_if_base.hh>

namespace mln
{

  /*! \brief An image class FIXME.
   *
   */
  template <typename I, typename F>
  struct image_if_value : public internal::image_if_base< I, F >
  {
    /// Skeleton.
    typedef image_if_value< tag::image_<I>, tag::function_<F> > skeleton;

    /// Constructor from an image \p ima and a predicate \p f.
    image_if_value(I& ima, const F& f);

    /// Constructor without argument.
    image_if_value();

    /// Const promotion via convertion.
    operator image_if_value<const I, F>() const;
  };

  // Operators.

  // Image | value.

  template <typename I>
  image_if_value< I,
	    fun::eq_p2b_expr_< pw::value_<I>,
			       pw::cst_<mln_value(I)> > >
  operator | (Image<I>& ima, const mln_value(I)& v);

  template <typename I>
  image_if_value< const I,
	    fun::eq_p2b_expr_< pw::value_<I>,
			       pw::cst_<mln_value(I)> > >
  operator | (const Image<I>& ima, const mln_value(I)& v);

# ifndef MLN_INCLUDE_ONLY

  // image_if_value<I,F>

  template <typename I, typename F>
  image_if_value<I,F>::image_if_value()
  {
  }

  template <typename I, typename F>
  image_if_value<I,F>::image_if_value(I& ima, const F& f)
  {
    this->init_(ima, f);
  }

  template <typename I, typename F>
  image_if_value<I,F>::operator image_if_value<const I, F>() const
  {
    image_if_value<const I, F> tmp(this->data_->ima_, this->data_->pset_);
    return tmp;
  }

  // Operators.

  template <typename I>
  image_if_value< I,
	    fun::eq_p2b_expr_< pw::value_<I>,
			       pw::cst_<mln_value(I)> > >
  operator | (Image<I>& ima, const mln_value(I)& v)
  {
    return ima | (pw::value(ima) == pw::cst(v));
  }

  template <typename I>
  image_if_value< const I,
	    fun::eq_p2b_expr_< pw::value_<I>,
			       pw::cst_<mln_value(I)> > >
  operator | (const Image<I>& ima, const mln_value(I)& v)
  {
    return ima | (pw::value(ima) == pw::cst(v));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IF_VALUE_HH
