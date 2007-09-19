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

# include <mln/core/internal/image_adaptor.hh>
# include <mln/value/proxy.hh>


namespace mln
{

  // Fwd decl.
  template <typename I, typename D> class decorated_image;


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

  } // end of namespace::internal



  // FIXME: Doc!

  template <typename I, typename D>
  class decorated_image : public internal::image_adaptor_< I, decorated_image<I,D> >,
			  public internal::decorated_image_impl_< I, decorated_image<I,D> >
  {
    typedef decorated_image<I, D> self_;
    typedef internal::image_adaptor_< I, self_ > super_;
    typedef internal::decorated_image_impl_< I, self_ > impl_;
  public:

    decorated_image(I& ima, const D& deco);
    ~decorated_image();

    typedef mln_value(I)                    value;
    typedef mln::value::proxy<const self_> rvalue;
    typedef typename impl_::lvalue         lvalue;


    /// Skeleton.
    typedef decorated_image< tag::image<I>, tag::data<D> > skeleton;


    rvalue operator()(const mln_psite(I)& p) const;
    lvalue operator()(const mln_psite(I)& p);

    mln_value(I) read_(const mln_psite(I)& p) const;

    /// Const promotion via convertion.
    operator decorated_image<const I, D>() const;

    /// Give the decoration.
    const D& decoration() const;

    /// Give the decoration.
    D& decoration();

  protected:
    D deco_;
  };



  template <typename I, typename D>
  decorated_image<I,D> decorate(Image<I>& ima,
				const D& decoration);

  template <typename I, typename D>
  decorated_image<const I, D> decorate(const Image<I>& ima,
				       const D& decoration);



# ifndef MLN_INCLUDE_ONLY

  // decorated_image<I,D>

  template <typename I, typename D>
  decorated_image<I,D>::decorated_image(I& ima, const D& deco)
    : super_(ima),
      deco_(deco)
  {
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

  template <typename I, typename D>
  typename decorated_image<I,D>::rvalue
  decorated_image<I,D>::operator()(const mln_psite(I)& p) const
  {
    rvalue tmp(*this, p);
    return tmp;
  }

  template <typename I, typename D>
  typename decorated_image<I,D>::lvalue
  decorated_image<I,D>::operator()(const mln_psite(I)& p)
  {
    lvalue tmp(*this, p);
    return tmp;
  }

  namespace internal
  {

    template <typename I, typename E>
    void
    decorated_image_impl_<I,E>::write_(const mln_psite(I)& p, const mln_value(I)& v)
    {
      E& ima = internal::force_exact<E>(*this);
      ima.decoration().writing(ima.adaptee(), p, v);
      ima.adaptee()(p) = v;
    }

  } // end of namespace mln::internal

  template <typename I, typename D>
  mln_value(I)
  decorated_image<I,D>::read_(const mln_psite(I)& p) const
  {
    deco_.reading(this->adaptee_, p);
    return this->adaptee_(p);
  }

  template <typename I, typename D>
  decorated_image<I,D>::operator decorated_image<const I, D>() const
  {
    decorated_image<const I, D> tmp(this->adaptee_, deco_);
    return tmp;
  }

  template <typename I, typename D>
  const D&
  decorated_image<I,D>::decoration() const
  {
    return deco_;
  }

  template <typename I, typename D>
  D&
  decorated_image<I,D>::decoration()
  {
    return deco_;
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
