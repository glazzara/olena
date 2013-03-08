// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMORPH_LAZY_IMAGE_HH
# define MLN_CORE_IMAGE_IMORPH_LAZY_IMAGE_HH

/// \file
///
/// Definition of a lazy image. Values are computed on the fly.


# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/core/alias/box2d.hh>


namespace mln
{

  // Forward declaration.
  template <typename I, typename F, typename B> struct lazy_image;

  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::lazy_image<I>.
    */
    template <typename I, typename F, typename B>
    struct data< lazy_image<I,F,B> >
    {
      data(const F& fun_, const B& box);

      mutable mln_ch_value(I,mln_result(F)) ima_;
      mutable mln_ch_value(I,bool)          is_known;
      const F&                              fun;
      const B&                              bb_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename F, typename B>
    struct image_< lazy_image<I,F,B> > : default_image_morpher< I, mln_value(I),
                                                                lazy_image<I,F,B> >
    {
      typedef trait::image::category::domain_morpher category;
      typedef trait::image::value_io::read_only value_io;
    };

  } // end of namespace mln::trait



  /*! \brief Image values are computed on the fly.
   *
   * The parameter \c I is the type of image.
   * The parameter \c F is the type of function.
   * The parameter \c B is the type of box.
   *
   * This image class tage a functor \p fun and a box \p box.
   * Access to ima(p) where \p p include \p box return fun(b) lazily.
   *
   * \ingroup modimageidmorpher
   */
  template <typename I, typename F, typename B>
  struct lazy_image :
    public mln::internal::image_identity< mln_ch_value(I, mln_result(F)),
                                          mln_domain(I), lazy_image<I, F,B> >
  {
    typedef mln::internal::image_identity< mln_ch_value(I, mln_result(F)),
                                           mln_domain(I),
                                           lazy_image<I, F,B> > super_;

    /// Return type of read access.
    typedef mln_result(F) rvalue;

    /// Return type of read-write access.
    typedef mln_result(F) lvalue;

    /// Skeleton.
    typedef lazy_image< tag::image_<I>, F, B > skeleton;

    /// Test if a pixel value is accessible at \p p.
    using super_::is_valid;

    /// Constructors.
    lazy_image();

    /// Constructors.
    lazy_image(const F& fun, const B& box);

    /// Initialize an empty image.
    void init_(const F& fun, const B& box);

    /// Return domain of lazyd_image.
    const box<mln_psite(I)>& domain() const;

    /// Test if a pixel value is accessible at \p p.
    bool has(const mln_psite(I)&) const;

    /// Read-only access of pixel value at F::input \p x.
    mln_result(F) operator()(const typename F::input& x) const;

    /// Read and "write if possible" access of pixel value at F::input \p x.
    mln_result(F) operator()(const typename F::input& x);

    /// Read-only access of pixel value at point site \p p.
    rvalue operator()(const mln_psite(I)& p) const;

    /// Read and "write if possible" access of pixel value at point site \p p.
    lvalue operator()(const mln_psite(I)& p);

  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< lazy_image<I,S> >

    template <typename I, typename F, typename B>
    inline
    data< lazy_image<I,F,B> >::data(const F& fun, const B& box)
      : ima_(box), is_known(box), fun(fun), bb_(box)
    {
    }

  } // end of namespace mln::internal

  template <typename I, typename F, typename B>
  inline
  lazy_image<I,F,B>::lazy_image(const F& fun, const B& box)
  {
    this->data_ = new internal::data< lazy_image<I,F,B> >(fun, box);
  }

  template <typename I, typename F, typename B>
  inline
  void lazy_image<I,F,B>::init_(const F& fun, const B& box)
  {
    this->data_ = new internal::data< lazy_image<I,F,B> >(fun, box);
  }

  template <typename I, typename F, typename B>
  inline
  bool lazy_image<I,F,B>::has(const mln_psite(I)& p) const
  {
    return this->data_->ima_.has(p);
  }

  template <typename I, typename F, typename B>
  inline
  mln_result(F)
  lazy_image<I,F,B>::operator()(const typename F::input& p) const
  {
    mln_assertion(this->has(p));
    if (this->data_->is_known(p))
      return this->data_->ima_(p);
    this->data_->ima_(p) = this->data_->fun(p);
    this->data_->is_known(p) = true;
    return this->data_->ima_(p);
  }


  template <typename I, typename F, typename B>
  inline
  mln_result(F)
  lazy_image<I,F,B>::operator()(const typename F::input& p)
  {
    mln_assertion(this->has(p));
    if (this->data_->is_known(p))
      return this->data_->ima_(p);
    this->data_->ima_(p) = this->data_->fun(p);
    this->data_->is_known(p) = true;
    return this->data_->ima_(p);
  }

  template <typename I, typename F, typename B>
  inline
  typename lazy_image<I,F,B>::rvalue
  lazy_image<I,F,B>::operator()(const mln_psite(I)& p) const
  {
    return (*this).operator()(convert::to< typename F::input >(p));
  }

  template <typename I, typename F, typename B>
  inline
  typename lazy_image<I,F,B>::lvalue
  lazy_image<I,F,B>::operator()(const mln_psite(I)& p)
  {
    return (*this).operator()(convert::to< typename F::input >(p));
  }

  template <typename I, typename F, typename B>
  inline
  const box<mln_psite(I)>&
  lazy_image<I,F,B>::domain() const
  {
    return this->data_->bb_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_LAZY_IMAGE_HH
