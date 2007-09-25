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

#ifndef MLN_CORE_DECORATED_IMAGE_HH
# define MLN_CORE_DECORATED_IMAGE_HH

# include <mln/core/internal/image_identity.hh>

# include <mln/value/proxy.hh>


namespace mln
{

  // Fwd decl.
  template <typename I, typename D> struct decorated_image;


  namespace internal
  {

    template <typename I, typename E>
    struct decorated_image_impl_
    {
      typedef mln::value::proxy<E> lvalue;
      void write_(const mln_psite(I)& p, const mln_value(I)& v);
    };

    template <typename I, typename E>
    struct decorated_image_impl_< const I, E >
    {
      typedef mln::value::proxy<const E> lvalue;
    };

    template <typename I, typename D>
    struct data_< decorated_image<I,D> >
    {
      data_(I& ima, const D& deco);

      I ima_;
      D deco_;
    };

  } // end of namespace mln::internal



  // FIXME: Doc!

  template <typename I, typename D>
  struct decorated_image :
    public internal::decorated_image_impl_< I, decorated_image<I,D> >,
    public internal::image_identity_< I, mln_pset(I), decorated_image<I,D> >
  {
  public:
    typedef decorated_image<I, D> self_;
    typedef internal::decorated_image_impl_< I, self_ > impl_;

    /// Return type of read-write access.
    typedef typename internal::morpher_lvalue_<I>::ret lvalue;

    /// Ctors
    decorated_image();
    decorated_image(I& ima, const D& deco);
    /// Dtor
    ~decorated_image();


    /// Value_Set associated type.
    typedef mln_vset(I) vset;

    /// Skeleton.
    typedef decorated_image< tag::image_<I>, tag::data_<D> > skeleton;

    mln_value(I) read_(const mln_psite(I)& p) const;

    /// Const promotion via convertion.
    operator decorated_image<const I, D>() const;

    /// Give the decoration.
    const D& decoration() const;

    /// Give the decoration.
    D& decoration();

  };



  template <typename I, typename D>
  decorated_image<I,D> decorate(Image<I>& ima,
				const D& decoration);

  template <typename I, typename D>
  decorated_image<const I, D> decorate(const Image<I>& ima,
				       const D& decoration);



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< decorated_image<I,S> >

    template <typename I, typename D>
    data_< decorated_image<I,D> >::data_(I& ima, const D& deco)
      : ima_(ima),
	deco_(deco)
    {
    }

  } // end of namespace mln::internal

  // decorated_image<I,D>

  template <typename I, typename D>
  decorated_image<I,D>::decorated_image()
  {
  }

  template <typename I, typename D>
  decorated_image<I,D>::decorated_image(I& ima, const D& deco)
  {
    this->data_ = new internal::data_< decorated_image<I,D> >(ima, deco);
  }

  template <typename I, typename D>
  decorated_image<I,D>::~decorated_image()
  {
    void (D::*mr)(const I&, const mln_psite(I)&) const = & D::reading;
    mr = 0;
    typedef mlc_unconst(I) I_;
    void (D::*mw)(I_&, const mln_psite(I_)&, const mln_value(I_)&) = & D::writing;
    mw = 0;
  }

  namespace internal
  {

    template <typename I, typename E>
    void
    decorated_image_impl_<I,E>::write_(const mln_psite(I)& p, const mln_value(I)& v)
    {
      E& ima = internal::force_exact<E>(*this);
      ima.decoration().writing(ima.data_->ima_, p, v);
      ima.data_->ima_(p) = v;
    }

  } // end of namespace mln::internal

  template <typename I, typename D>
  mln_value(I)
    decorated_image<I,D>::read_(const mln_psite(I)& p) const
  {
    this->data_->deco_.reading(this->data_->ima_, p);
    return this->data_->ima_(p);
  }

  template <typename I, typename D>
  decorated_image<I,D>::operator decorated_image<const I, D>() const
  {
    decorated_image<const I, D> tmp(this->data_->ima_, this->data_->deco_);
    return tmp;
  }


  template <typename I, typename D>
  const D&
  decorated_image<I,D>::decoration() const
  {
    return this->data_->deco_;
  }

  template <typename I, typename D>
  D&
  decorated_image<I,D>::decoration()
  {
    return this->data_->deco_;
  }

  // decorate

  template <typename I, typename D>
  decorated_image<I, D> decorate(Image<I>& ima,
				 const D& decoration)
  {
    decorated_image<I, D> tmp(exact(ima), decoration);
    return tmp;
  }

  template <typename I, typename D>
  decorated_image<const I, D> decorate(const Image<I>& ima,
				       const D& decoration)
  {
    decorated_image<const I, D> tmp(exact(ima), decoration);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_DECORATED_IMAGE_HH
