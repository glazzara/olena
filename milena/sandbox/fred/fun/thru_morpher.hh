// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef THRU_HH
# define THRU_HH

# include <mln/core/internal/image_value_morpher.hh>
# include "meta_function.hh"

// FIXME: constness of thru_image

namespace mln
{

  // Forward declaration.
  template <typename I, typename F> struct thru_image;


  namespace internal
  {

    /// Data structure for \c mln::thru_image<I>.
    template <typename I, typename F>
    struct data< thru_image<I, F> >
    {
      data(I& ima, const F& f);

      I ima_;
      // FIXME: value or pointer or whatever ?
      F f_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename F>
    struct image_< thru_image<I, F> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::value_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::computed value_access;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename I, typename F>
  class thru_image : public internal::image_value_morpher< I, typename F::result, thru_image<I,F> >
  {
  public:

    /// Skeleton.
    typedef thru_image<tag::image_<I>, F> skeleton;
    
    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef typename F::result value;

    /// Type returned by the read-write pixel value operator.
    typedef typename F::lresult lvalue;

    /// Return type of read-only access.
    typedef typename F::result rvalue;
    
    thru_image();
    thru_image(I& ima);
    thru_image(I& ima, const F& f);

    // Initialize an empty image.
    void init_(I& ima, const F& f);

    rvalue operator()(const mln_psite(I)& p) const;

    lvalue operator()(const mln_psite(I)& p);

    /// Const promotion via conversion.
    operator thru_image<const I, F>() const;
  };

  template <typename I, typename F>
  thru_image<I, F> thru(const mln::Function<F>& f,
			Image<I>& ima);

  template <typename I, typename F>
  thru_image<const I, F> thru(const mln::Function<F>& f,
			      const Image<I>& ima);

  template <typename I, typename M>
  thru_image<I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, Image<I>& ima);

  template <typename I, typename M>
  thru_image<const I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, const Image<I>& ima);

# ifndef MLN_INCLUDE_ONLY

  // internal::data< thru_image<I,S> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< thru_image<I, F> >::data(I& ima, const F& f)
      : ima_(ima),
	f_(f)
    {
    }

  } // end of namespace mln::internal

  // thru_image<I>

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image()
  {
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image(I& ima, const F& f)
  {
    mln_precondition(ima.is_valid());
    init_(ima, f);
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::thru_image(I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima, mln_value(I)());
  }

  template <typename I, typename F>
  inline
  void
  thru_image<I, F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->is_valid());
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< thru_image<I, F> >(ima, f);
  }

  template <typename I, typename F>
  inline
  typename thru_image<I, F>::rvalue
  thru_image<I, F>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->f_(this->data_->ima_(p));
  }

  template <typename I, typename F>
  inline
  typename thru_image<I, F>::lvalue
  thru_image<I, F>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->is_valid());
    return this->data_->f_(this->data_->ima_(p));    
  }

  template <typename I, typename F>
  inline
  thru_image<I, F>::operator thru_image<const I, F>() const
  {
    thru_image<const I, F> tmp(this->data_->ima_, this->data_->default_value_);
    return tmp;
  }

  // thru  
  template <typename I, typename F>
  thru_image<I, F> thru(const mln::Function<F>& f,
			Image<I>& ima)
  {
    thru_image<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  thru_image<const I, F> thru(const mln::Function<F>& f,
			      const Image<I>& ima)
  {
    thru_image<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename M>
  thru_image<I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thru_image<I, F> tmp(exact(ima), F());

    return tmp;
  }

  template <typename I, typename M>
  thru_image<const I, mln_fun_with(M, mln_value(I))>
  thru(const mln::Meta_Function<M>& f, const Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thru_image<const I, F> tmp(exact(ima), F());

    return tmp;
  }
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! THRU_HH
