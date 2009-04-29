// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
/// Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_THRUBIN_MORPHER_HH
# define MLN_CORE_IMAGE_THRUBIN_MORPHER_HH

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/core/concept/meta_function.hh>
# include <mln/metal/bexpr.hh>
# include <mln/trait/fun.hh>

/// \file mln/core/image/thrubin_morpher.hh
///
/// \brief Definition of a morpher that morph values from two images
/// through a binary function.

namespace mln
{

  // Forward declaration.
  template <typename I, typename F> struct thrubin_image;

  namespace internal
  {
    /// Data structure for \c mln::thrubin_image<I>.
    template <typename I1, typename I2, typename F>
    struct data< thrubin_image<I1, I2, F> >
    {
      data(I1& ima1, I2& ima2, const F& f);

      I1 ima1_;
      I2 ima2_;
      F f_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I1, typename I2, typename F>
    struct image_< thrubin_image<I1, I2, F> > : image_< typename mln::internal::thrubin_find_impl<I1, , F>::ret > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::value_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::computed value_access;
      typedef trait::image::vw_io::read vw_io;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  namespace internal
  {

    template <typename I, typename F>
    class thrubin_image_read : public internal::image_value_morpher< I, typename F::result, thrubin_image<I,F> >
    {
    public:

      /// Skeleton.
      typedef thrubin_image<tag::image_<I>, F> skeleton;

      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Value associated type.
      typedef typename F::result value;

      /// Return type of read-only access.
      typedef typename F::result rvalue;

      rvalue operator()(const mln_psite(I)& p) const;

    };

    // Inheritance from read ?!
    template <typename I, typename F>
    class thrubin_image_write : public thrubin_image_read<I,F>
    {
      public:

	/// Type returned by the read-write pixel value operator.
// 	typedef typename F::template lresult<typename F::argument>::ret lvalue;
	typedef typename F::lresult lvalue;

	using thrubin_image_read<I,F>::operator();
	lvalue operator()(const mln_psite(I)& p);

    };
  }

  /// \brief Morphes values from two images through a binary function.
  ///
  /// \ingroup modimagevaluemorpher
  template <typename I, typename F>
  class thrubin_image : public internal::thrubin_find_impl<I, F>::ret
  {
  public:

    thrubin_image();
    thrubin_image(I& ima);
    thrubin_image(I& ima, const F& f);

    void init_(I& ima, const F& f);

    /// Const promotion via conversion.
    operator thrubin_image<const I, F>() const;
  };

  template <typename I, typename F>
  thrubin_image<I, F> thrubin(const mln::Function<F>& f,
			Image<I>& ima);

  template <typename I, typename F>
  const thrubin_image<const I, F> thrubin(const mln::Function<F>& f,
				    const Image<I>& ima);

  template <typename I, typename M>
  thrubin_image<I, mln_fun_with(M, mln_value(I))>
  thrubin(const mln::Meta_Function<M>& f, Image<I>& ima);

  template <typename I, typename M>
  const thrubin_image<const I, mln_fun_with(M, mln_value(I))>
  thrubin(const mln::Meta_Function<M>& f, const Image<I>& ima);

# ifndef MLN_INCLUDE_ONLY

  // internal::data< thrubin_image<I,S> >

  namespace internal
  {

    template <typename I, typename F>
    inline
    data< thrubin_image<I, F> >::data(I& ima, const F& f)
      : ima_(ima),
	f_(f)
    {
    }

  } // end of namespace mln::internal

  // thrubin_image<I>

  template <typename I, typename F>
  inline
  thrubin_image<I, F>::thrubin_image()
  {
  }

  template <typename I, typename F>
  inline
  thrubin_image<I, F>::thrubin_image(I& ima, const F& f)
  {
    mln_precondition(ima.is_valid());
    init_(ima, f);
  }

  template <typename I, typename F>
  inline
  thrubin_image<I, F>::thrubin_image(I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima, mln_value(I)());
  }

  template <typename I, typename F>
  inline
  void
  thrubin_image<I, F>::init_(I& ima, const F& f)
  {
    mln_precondition(! this->is_valid());
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< thrubin_image<I, F> >(ima, f);
  }

  template <typename I, typename F>
  inline
  thrubin_image<I, F>::operator thrubin_image<const I, F>() const
  {
    thrubin_image<const I, F> tmp(this->data_->ima_, this->data_->f_);
    return tmp;
  }

  namespace internal
  {

    template <typename I, typename F>
    inline
    typename thrubin_image_read<I, F>::rvalue
    thrubin_image_read<I, F>::operator()(const mln_psite(I)& p) const
    {
      mln_precondition(this->is_valid());
      return this->data_->f_(this->data_->ima_(p));
    }

    template <typename I, typename F>
    inline
    typename thrubin_image_write<I, F>::lvalue
    thrubin_image_write<I, F>::operator()(const mln_psite(I)& p)
    {
      mln_precondition(this->is_valid());
      return this->data_->f_(this->data_->ima_(p));
    }

  }

  // thrubin
  template <typename I, typename F>
  thrubin_image<I, F> thrubin(const mln::Function<F>& f,
			Image<I>& ima)
  {
    thrubin_image<I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename F>
  thrubin_image<const I, F> thrubin(const mln::Function<F>& f,
			      const Image<I>& ima)
  {
    thrubin_image<const I, F> tmp(exact(ima), exact(f));
    return tmp;
  }

  template <typename I, typename M>
  thrubin_image<I, mln_fun_with(M, mln_value(I))>
  thrubin(const mln::Meta_Function<M>& f, Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thrubin_image<I, F> tmp(exact(ima), F());

    return tmp;
  }

  template <typename I, typename M>
  thrubin_image<const I, mln_fun_with(M, mln_value(I))>
  thrubin(const mln::Meta_Function<M>& f, const Image<I>& ima)
  {
    typedef mln_fun_with(M, mln_value(I)) F;
    thrubin_image<const I, F> tmp(exact(ima), F());

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_THRUBIN_MORPHER_HH
