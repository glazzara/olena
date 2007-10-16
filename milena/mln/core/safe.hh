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

#ifndef MLN_CORE_SAFE_HH
# define MLN_CORE_SAFE_HH

/*!
 * \file   safe.hh
 *
 * \brief Definition of a morpher that makes image become accessible
 * at undefined location.
 *
 */


# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Fwd decl.
  template <typename I> struct safe_image;

  namespace internal
  {

    template <typename I>
    struct data_< safe_image<I> >
    {
      data_(I& ima, const mln_value(I)& default_value);

      I ima_;
      mln_value(I) default_value_;
    };

  } // end of namespace mln::internal


  // FIXME: Doc!

  template <typename I>
  class safe_image : public internal::image_identity_< I, mln_pset(I), safe_image<I> >
  {
    typedef internal::image_identity_< I, mln_pset(I), safe_image<I> > super_;
  public:

    /// Skeleton.
    typedef safe_image< tag::image_<I> > skeleton;

    safe_image(I& ima, const mln_value(I)& default_value);
    safe_image();

    mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    typedef typename super_::lvalue lvalue;
    lvalue operator()(const mln_psite(I)& p);

    /// Const promotion via convertion.
    operator safe_image<const I>() const;
  };



  template <typename I>
  safe_image<I> safe(Image<I>& ima,
		     mln_value(I) default_value = mln_value(I)());

  template <typename I>
  safe_image<const I> safe(const Image<I>& ima,
			   mln_value(I) default_value = mln_value(I)());



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< safe_image<I,S> >

    template <typename I>
    data_< safe_image<I> >::data_(I& ima, const mln_value(I)& default_value)
      : ima_(ima),
	default_value_(default_value)
    {
    }

  } // end of namespace mln::internal

  // safe_image<I>

  template <typename I>
  safe_image<I>::safe_image(I& ima, const mln_value(I)& default_value)
  {
    this->data_ = new internal::data_< safe_image<I> >(ima, default_value);
  }

  template <typename I>
  safe_image<I>::safe_image()
  {
  }

  template <typename I>
  mln_rvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p) const
  {
    if (! this->owns_(p))
      return this->data_->default_value_;
    return this->data_->ima_(p);
  }

  template <typename I>
  typename safe_image<I>::lvalue
  safe_image<I>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) forget_it_;
    if (! this->owns_(p))
      // so data_->default_value_ is returned but cannot be modified
      return forget_it_ = this->data_->default_value_;
    return this->data_->ima_(p);
  }

  template <typename I>
  safe_image<I>::operator safe_image<const I>() const
  {
    safe_image<const I> tmp(this->data_->ima_, this->data_->default_value_);
    return tmp;
  }

  // safe

  template <typename I>
  safe_image<I> safe(Image<I>& ima,
		     mln_value(I) default_value)
  {
    safe_image<I> tmp(exact(ima), default_value);
    return tmp;
  }

  template <typename I>
  safe_image<const I> safe(const Image<I>& ima,
			   mln_value(I) default_value)
  {
    safe_image<const I> tmp(exact(ima), default_value);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SAFE_HH
