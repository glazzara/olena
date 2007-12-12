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
 * \brief Definition of a image which domain is restricted by a
 * single value.
 */

# include <mln/core/internal/image_if_base.hh>
# include <mln/metal/unconst.hh>


# define  F      fun::eq_p2b_expr_< pw::value_<mlc_unconst(I)>, pw::cst_<mln_value(I)> >
# define  Super  mln::internal::image_if_base_< I, F, image_if_value<I> >


namespace mln
{

  // Fwd decl.
  template <typename I> struct image_if_value;


  /// \internal internal::data_.

  namespace internal
  {

    template <typename I>
    struct data_< image_if_value<I> > : data_< Super >
    {
      data_(I& ima, const F& f);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< image_if_value<I> > : trait::image_< Super >
    {
    };

  } // end of namespace mln::trait



  /*! \brief Image which domain is restricted by a single value.
   *
   */
  template <typename I>
  struct image_if_value : public Super
  {
    /// Skeleton.
    typedef image_if_value< tag::image_<I> > skeleton;

    /// Natural constructor from an image \p ima and a value \p v.
    image_if_value(I& ima, const mln_value(I)& v);

    /// Ancestral constructor from an image \p ima and a function \p f.
    image_if_value(I& ima, const F& f);

    /// Constructor without argument.
    image_if_value();

    // FIXME: Conversion below does *not* work automatically.
    /// Const promotion via conversion.
    operator image_if_value<const I> () const;
  };



  // init_.

  template <typename I>
  void init_(tag::function_t, F& f, const image_if_value<I>& model)
  {
    f = model.domain().predicate();
  }

  template <typename I, typename J>
  void init_(tag::image_t, image_if_value<I>& target, const J& model)
  {
    I ima;
    init_(tag::image, ima, exact(model));
    F f;
    init_(tag::function, f, exact(model));
    target.init_(ima, f);
  }



  // Operators.

  // Image | value.

  /// ima | v creates an image_if_value with the image ima and the
  /// value v.
  template <typename I>
  image_if_value<I>
  operator | (Image<I>& ima, const mln_value(I)& v);

  /// ima | v creates an image_if_value with the image ima and the
  /// value v.
  template <typename I>
  image_if_value<const I>
  operator | (const Image<I>& ima, const mln_value(I)& v);


# ifndef MLN_INCLUDE_ONLY

  // image_if_value<I>

  template <typename I>
  inline
  image_if_value<I>::image_if_value()
  {
  }

  template <typename I>
  inline
  image_if_value<I>::image_if_value(I& ima, const F& f)
  {
    this->init_(ima, f);
  }

  template <typename I>
  inline
  image_if_value<I>::image_if_value(I& ima, const mln_value(I)& v)
  {
    this->init_(ima, pw::value(ima) == pw::cst(v));
  }

  template <typename I>
  inline
  image_if_value<I>::operator image_if_value<const I>() const
  {
    mln_precondition(this->has_data());
    image_if_value<const I> tmp(this->data_->ima_,
				this->data_->pset_.predicate);
    return tmp;
  }

  // internal::data_< image_if_value<I> >

  namespace internal
  {

    template <typename I>
    inline
    data_< image_if_value<I> >::data_(I& ima, const F& f)
      : data_< Super >(ima, f)
    {
    }

  }

  // Operators.

  template <typename I>
  inline
  image_if_value<I>
  operator | (Image<I>& ima, const mln_value(I)& v)
  {
    image_if_value<I> tmp(exact(ima), v);
    return tmp;
  }

  template <typename I>
  inline
  image_if_value<const I>
  operator | (const Image<I>& ima, const mln_value(I)& v)
  {
    image_if_value<const I> tmp(exact(ima), v);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# undef Super
# undef F


#endif // ! MLN_CORE_IMAGE_IF_VALUE_HH
