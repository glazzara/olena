// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_IMAGE_IMORPH_DECORATED_IMAGE_HH
# define MLN_CORE_IMAGE_IMORPH_DECORATED_IMAGE_HH

# include <mln/core/internal/image_identity.hh>

# include <mln/value/proxy.hh>

/// \file
///
/// Definition of an image that can have additional features.


namespace mln
{

  // Forward declaration.
  template <typename I, typename D> struct decorated_image;


  namespace internal
  {

    template <typename I, typename E>
    struct decorated_image_impl_
    {
      typedef mln::value::proxy<E> lvalue;
      /// Actual write routine.
      void write_(const mln_psite(I)& p, const mln_value(I)& v);
    };

    template <typename I, typename E>
    struct decorated_image_impl_< const I, E >
    {
      typedef mln::value::proxy<const E> lvalue;
    };

    /// Data structure for \c mln::decorated_image<I,D>.
    template <typename I, typename D>
    struct data< decorated_image<I,D> >
    {
      data(I& ima, const D& deco);

      I ima_;
      D deco_;
    };

  } // end of namespace mln::internal



  /// Image that can have additional features.
  ///
  /// \ingroup modimageidmorpher
  //
  template <typename I, typename D>
  struct decorated_image :
    public internal::decorated_image_impl_< I, decorated_image<I,D> >,
    public internal::image_identity< I, mln_domain(I), decorated_image<I,D> >
  {
  public:
    typedef decorated_image<I, D> self_;
    typedef internal::decorated_image_impl_< I, self_ > impl_;

    /// Type of the psite.
    typedef mln_psite(I) psite;

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;
    /// Return type of read-write access.
    typedef typename impl_::lvalue lvalue;

    /// Ctors
    decorated_image();
    decorated_image(I& ima, const D& deco);

    /// Initialize an empty image.
    void init_(I& ima, const D& deco);

    /// Dtor
    ~decorated_image();

    /// Skeleton.
    typedef decorated_image< tag::image_<I>, tag::data_<D> > skeleton;

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const psite& p) const;

    /// Read-write access of pixel value at point site \p p.
    lvalue operator()(const psite& p);

    /// Actual read routine.
    rvalue read_(const mln_psite(I)& p) const;

    /// Const promotion via conversion.
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

    /// internal::data< decorated_image<I,S> >
    template <typename I, typename D>
    inline
    data< decorated_image<I,D> >::data(I& ima, const D& deco)
      : ima_(ima),
	deco_(deco)
    {
    }

  } // end of namespace mln::internal

  // decorated_image<I,D>

  template <typename I, typename D>
  inline
  decorated_image<I,D>::decorated_image()
  {
  }

  template <typename I, typename D>
  inline
  decorated_image<I,D>::decorated_image(I& ima, const D& deco)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data< decorated_image<I,D> >(ima, deco);
  }

  template <typename I, typename D>
  inline
  void
  decorated_image<I,D>::init_(I& ima, const D& deco)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data<decorated_image<I,D> >(ima, deco);
  }

  template <typename I, typename D>
  inline
  decorated_image<I,D>::~decorated_image()
  {
    void (D::*mr)(const I&, const mln_psite(I)&) const = & D::reading;
    (void) mr;
    typedef mlc_unconst(I) I_;
    void (D::*mw)(I_&, const mln_psite(I_)&, const mln_value(I_)&) =
      & D::writing;
    (void) mw;
  }

  template <typename I, typename D>
  inline
  typename decorated_image<I,D>::rvalue
  decorated_image<I,D>::operator()(const psite& p) const
  {
    mln_precondition(this->delegatee_() != 0);
    return read_(p);
  }

  template <typename I, typename D>
  inline
  typename decorated_image<I,D>::lvalue
  decorated_image<I,D>::operator()(const psite& p)
  {
    mln_precondition(this->delegatee_() != 0);
    // Return a proxy.
    return lvalue(*this, p);
  }

  template <typename I, typename D>
  inline
  mln_rvalue(I)
  decorated_image<I,D>::read_(const mln_psite(I)& p) const
  {
    this->data_->deco_.reading(this->data_->ima_, p);
    return this->data_->ima_(p);
  }

  namespace internal
  {
    template <typename I, typename E>
    inline
    void
    decorated_image_impl_<I,E>::write_(const mln_psite(I)& p,
				       const mln_value(I)& v)
    {
      E& ima = internal::force_exact<E>(*this);
      ima.decoration().writing(*ima.delegatee_(), p, v);
      (*ima.delegatee_())(p) = v;
    }

  } // end of namespace mln::internal

  template <typename I, typename D>
  inline
  decorated_image<I,D>::operator decorated_image<const I, D>() const
  {
    decorated_image<const I, D> tmp(this->data_->ima_, this->data_->deco_);
    return tmp;
  }


  template <typename I, typename D>
  inline
  const D&
  decorated_image<I,D>::decoration() const
  {
    return this->data_->deco_;
  }

  template <typename I, typename D>
  inline
  D&
  decorated_image<I,D>::decoration()
  {
    return this->data_->deco_;
  }

  // decorate

  template <typename I, typename D>
  inline
  decorated_image<I, D> decorate(Image<I>& ima,
				 const D& decoration)
  {
    decorated_image<I, D> tmp(exact(ima), decoration);
    return tmp;
  }

  template <typename I, typename D>
  inline
  decorated_image<const I, D> decorate(const Image<I>& ima,
				       const D& decoration)
  {
    decorated_image<const I, D> tmp(exact(ima), decoration);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_DECORATED_IMAGE_HH
