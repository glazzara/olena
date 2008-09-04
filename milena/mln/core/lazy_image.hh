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

#ifndef MLN_CORE_LAZY_IMAGE_HH
# define MLN_CORE_LAZY_IMAGE_HH

/*! \file mln/core/lazy_image.hh
 *
 * \brief Definition of a lazy image. Values are computed on the fly
 */

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/line_piter.hh>


namespace mln
{

  // Fwd decl.
  template <typename I, typename F, typename B> struct lazy_image;

  namespace internal
  {

    /// \internal Data structure for \c mln::lazy_image<I>.
    template <typename I, typename F, typename B>
    struct data_< lazy_image<I,F,B> >
    {
      data_(const F& fun_, const B& box);

      mutable mln_ch_value(I,mln_result(F)) ima_;
      mutable mln_ch_value(I,bool)          is_known;
      const F&                              fun;
      const B&                              bb_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename F, typename B>
    struct image_< lazy_image<I,F,B> > : default_image_morpher_< I, mln_value(I),
                                                             lazy_image<I,F,B> >
    {
      typedef trait::image::category::domain_morpher category;

      typedef mln_trait_image_access(I)     access;
      typedef mln_trait_image_space(I)      space;
      typedef mln_trait_image_size(I)       size;
      typedef mln_trait_image_support(I)    support;
      typedef mln_trait_image_border(I)     border;
      typedef mln_trait_image_io_from_(I)   io;
      typedef mln_trait_image_data_from_(I) data;

    };

  } // end of namespace mln::trait



  /*! \brief Lazy image class.
   *
   * The parameter \c I is the type of image.
   * The parameter \c F is the type of function.
   * The parameter \c B is the type of box.
   *
   * This image class tage a functor \p fun and a box \p box.
   * Access to ima(p) where \p p include \p box return fun(b) lazily.
   * 
   */
  template <typename I, typename F, typename B>
  struct lazy_image : public mln::internal::image_identity_< mln_ch_value(I, mln_result(F)),
                                                             mln_pset(I), lazy_image<I, F,B> >
  {
    typedef mln::internal::image_morpher_< mln_ch_value(I, mln_result(F)),
                                           mln_pset(I), lazy_image<I,F,B> > super_;
    typedef line_piter_<mln_psite(I)> line_piter;

    /// Return type of read access.
    typedef mln_result(F) rvalue;
    
    /// Return type of read-write access.
    typedef mln_result(F)& lvalue;

    /// Skeleton.
    typedef lazy_image< tag::image_<I>, F, B > skeleton;

    /// Test if a pixel value is accessible at \p p.
    using super_::has_data;

    /// Constructors.
    lazy_image(const F& fun, const B& box);

    /// Return domain of lazyd_image.
    const box2d& domain() const;

    /// Test if a pixel value is accessible at \p p.
    bool owns_(const mln_psite(I)&) const;

    /// Read-only access of pixel value at point site \p p.
    mln_result(F) operator()(const mln_psite(I)& p) const;

    /// Read and "write if possible" access of pixel value at point site \p p.
    lvalue operator()(const mln_psite(I)& p);
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< lazy_image<I,S> >

    template <typename I, typename F, typename B>
    inline
    data_< lazy_image<I,F,B> >::data_(const F& fun, const B& box)
      : ima_(box), is_known(box), fun(fun), bb_(box)
    {
    }

  } // end of namespace mln::internal

  template <typename I, typename F, typename B>
  inline
  lazy_image<I,F,B>::lazy_image(const F& fun, const B& box)
  {
    this->data_ = new internal::data_< lazy_image<I,F,B> >(fun, box);
  }

  template <typename I, typename F, typename B>
  inline
  bool lazy_image<I,F,B>::owns_(const mln_psite(I)& p) const
  {
    return this->data_->ima_.owns_(p);
  }

  template <typename I, typename F, typename B>
  inline
  mln_result(F)
  lazy_image<I,F,B>::operator()(const mln_psite(I)& p) const
  {
    mln_assertion(this->owns_(p));
    if (this->data_->is_known(p))
      return this->data_->ima_(p);
    this->data_->ima_(p) = this->data_->fun(p);
    this->data_->is_known(p) = true;
    return this->data_->ima_(p);
  }


  template <typename I, typename F, typename B>
  inline
  typename lazy_image<I,F,B>::lvalue
  lazy_image<I,F,B>::operator()(const mln_psite(I)& p)
  {
    mln_assertion(this->owns_(p));
    if (this->data_->is_known(p))
      return this->data_->ima_(p);
    this->data_->ima_(p) = this->data_->fun(p);
    this->data_->is_known(p) = true;
    return this->data_->ima_(p);
  }

  template <typename I, typename F, typename B>
  inline
  const box2d&
  lazy_image<I,F,B>::domain() const
  {
    return this->data_->bb_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LAZY_IMAGE_HH
