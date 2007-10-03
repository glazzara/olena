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

#ifndef MLN_CORE_IMAGE_IF_INTERVAL_HH
# define MLN_CORE_IMAGE_IF_INTERVAL_HH

/*! \file mln/core/image_if_interval.hh
 *
 * \brief Definition of a image_if_interval image.
 */

# include <mln/core/internal/image_if_base.hh>
# include <mln/metal/unconst.hh>
# include <mln/value/interval.hh>


# define  F	 fun::and_p2b_expr_<					       \
			fun::geq_p2b_expr_<				       \
				pw::value_<I>,				       \
				pw::cst_<mln_value(I)> >,		       \
			fun::leq_p2b_expr_<				       \
				pw::value_<I>,				       \
				pw::cst_<mln_value(I)> > >

# define  Super  mln::internal::image_if_base_< I, F, image_if_interval<I> >


namespace mln
{

  // Fwd decl.
  template <typename I> struct image_if_interval;


  // internal::data_.

  namespace internal
  {

    template <typename I>
    struct data_< image_if_interval<I> > : data_< Super >
    {
	data_(I& ima, const F& f);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< image_if_interval<I> > : trait::image_< Super >
    {
    };

  } // end of namespace mln::trait



  /*! \brief An image class FIXME.
   *
   */
  template <typename I>
  struct image_if_interval : public Super
  {
    /// Skeleton.
    typedef image_if_interval< tag::image_<I> > skeleton;

    /// Constructor from an image \p ima and a predicate \p f.
    image_if_interval(I& ima, const F& f);

    /// Natural constructor from an image \p ima and a interval of value \p vv.
    image_if_interval(I& ima, const value::interval_<mln_value(I)>&);

    /// Constructor without argument.
    image_if_interval();

    /// Const promotion via convertion.
    operator image_if_interval<const I>() const;
  };

  // Operators.

  // Image | [from, to].

  template <typename I>
  image_if_interval<I>
  operator | (Image<I>& ima, const value::interval_<mln_value(I)>& vv);

  template <typename I>
  image_if_interval<const I>
  operator | (const Image<I>& ima, const value::interval_<mln_value(I)>& vv);



# ifndef MLN_INCLUDE_ONLY

  // image_if_interval<I>

  template <typename I>
  image_if_interval<I>::image_if_interval()
  {
  }

  template <typename I>
  image_if_interval<I>::image_if_interval(I& ima, const F& f)
  {
    this->init_(ima, f);
  }

  template <typename I>
  image_if_interval<I>::image_if_interval(I& ima, const value::interval_<mln_value(I)>& vv)
  {
    this->init_(ima, (pw::value(ima) >= pw::cst(vv.from))
		&& (pw::value(ima) <= pw::cst(vv.to)));
  }

  template <typename I>
  image_if_interval<I>::operator image_if_interval<const I>() const
  {
    image_if_interval<const I> tmp(this->data_->ima_, this->data_->pset_);
    return tmp;
  }

  // internal::data_< image_if_interval<I> >

  namespace internal
  {

    template <typename I>
    data_< image_if_interval<I> >::data_(I& ima, const F& f)
      : data_< Super >(ima, f)
    {
    }

  } // end of namespace mln::internal

  // Operators.

  template <typename I>
  image_if_interval<I>
  operator | (Image<I>& ima, const value::interval_<mln_value(I)>& vv)
  {
    image_if_interval<I> tmp(exact(ima), vv);
    return tmp;
  }

  template <typename I>
  image_if_interval<const I>
  operator | (const Image<I>& ima, const value::interval_<mln_value(I)>& vv)
  {
    image_if_interval<const I> tmp(exact(ima), vv);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# undef Super
# undef F

#endif // ! MLN_CORE_IMAGE_IF_INTERVAL_HH
