// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_VMORPH_THRUBIN_IMAGE_HH
# define MLN_CORE_IMAGE_VMORPH_THRUBIN_IMAGE_HH

/// \file
///
/// \brief Definition of a morpher that morph values from two images
/// through a binary function.

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/core/concept/meta_function.hh>
# include <mln/metal/bexpr.hh>
# include <mln/trait/functions.hh>


namespace mln
{

  // Forward declaration.
  template <typename I1, typename I2, typename F> class thrubin_image;

  namespace internal
  {
    /// Data structure for \c mln::thrubin_image<I>.
    template <typename I1, typename I2, typename F>
    struct data< thrubin_image<I1, I2, F> >
    {
      data(I1& ima1, I2& ima2, const F& f);

      I1 ima_;
      I2 ima2_;
      F f_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I1, typename I2, typename F>
    struct image_< thrubin_image<I1, I2, F> > : image_< I1 > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::value_morpher category;
      typedef mln_internal_trait_image_speed_from(I1) speed; // Un-fastest.
      typedef trait::image::value_access::computed value_access;
      typedef trait::image::vw_io::read vw_io;
    };

  } // end of namespace mln::trait

  /// Morphes values from two images through a binary function.
  ///
  /// \ingroup modimagevaluemorpher
  template <typename I1, typename I2, typename F>
  class thrubin_image : public internal::image_value_morpher< I1, typename F::result, thrubin_image<I1,I2,F> >
  {
  public:

    thrubin_image();
    thrubin_image(I1& ima1, I2& ima2);
    thrubin_image(I1& ima1, I2& ima2, const F& f);

    /// Skeleton.
    typedef thrubin_image<tag::image_<I1>, tag::image_<I2>, F> skeleton;

    /// Point_Site associated type.
    typedef mln_psite(I1) psite;

    /// Value associated type.
    typedef mln_result(F) value;

    /// Return type of read-only access.
    typedef value rvalue;
    typedef value lvalue; // Workaround for write operator()

    rvalue operator()(const mln_psite(I1)& p) const;
    rvalue operator()(const mln_psite(I1)& p);

    void init_(I1& ima1, I2& ima2, const F& f);

    /// Const promotion via conversion.
    operator thrubin_image<const I1, const I2, F>() const;
  };

  template <typename I1, typename I2, typename F>
  thrubin_image<I1, I2, F>
  thru(const mln::Function<F>& f,
       Image<I1>& ima1, Image<I2>& ima2);

  template <typename I1, typename I2, typename F>
  const thrubin_image<const I1, const I2, F>
  thru(const mln::Function<F>& f,
       const Image<I1>& ima1, const Image<I2>& ima2);

  template <typename I1, typename I2, typename M>
  thrubin_image<I1, I2, mln_fun_withbin(M, mln_value(I1), mln_value(I2))>
  thru(const mln::Meta_Function<M>& f, Image<I1>& ima1, Image<I1>& ima2);

  template <typename I1, typename I2, typename M>
  const thrubin_image<const I1, const I2, mln_fun_withbin(M, mln_value(I1), mln_value(I2))>
  thru(const mln::Meta_Function<M>& f, const Image<I1>& ima1, const Image<I2>& ima2);

# ifndef MLN_INCLUDE_ONLY

  // internal::data< thrubin_image<I,S> >

  namespace internal
  {

    template <typename I1, typename I2, typename F>
    inline
    data< thrubin_image<I1, I2, F> >::data(I1& ima1, I2& ima2, const F& f)
      : ima_(ima1), ima2_(ima2),
	f_(f)
    {
    }

  } // end of namespace mln::internal

  // thrubin_image<I>

  template <typename I1, typename I2, typename F>
  inline
  thrubin_image<I1, I2, F>::thrubin_image()
  {
  }

  template <typename I1, typename I2, typename F>
  inline
  thrubin_image<I1, I2, F>::thrubin_image(I1& ima1, I2& ima2, const F& f)
  {
    mln_precondition(ima1.is_valid());
    mln_precondition(ima2.is_valid());
    init_(ima1, ima2, f);
  }

  template <typename I1, typename I2, typename F>
  inline
  thrubin_image<I1, I2, F>::thrubin_image(I1& ima1, I2& ima2)
  {
    mln_precondition(ima1.is_valid());
    mln_precondition(ima2.is_valid());
    init_(ima1, ima2, F());
  }

  template <typename I1, typename I2, typename F>
  inline
  void
  thrubin_image<I1, I2, F>::init_(I1& ima1, I2& ima2, const F& f)
  {
    mln_precondition(! this->is_valid());
    mln_precondition(ima1.is_valid());
    mln_precondition(ima2.is_valid());
    this->data_ = new internal::data< thrubin_image<I1, I2, F> >(ima1, ima2, f);
  }

  template <typename I1, typename I2, typename F>
  inline
  thrubin_image<I1, I2, F>::operator thrubin_image<const I1, const I2, F>() const
  {
    thrubin_image<const I1, const I2, F> tmp(this->data_->ima_, this->data_->ima2_, this->data_->f_);
    return tmp;
  }

  template <typename I1, typename I2, typename F>
  inline
  typename thrubin_image<I1, I2, F>::rvalue
  thrubin_image<I1, I2, F>::operator()(const mln_psite(I1)& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->f_(this->data_->ima_(p), this->data_->ima2_(p));
  }

  template <typename I1, typename I2, typename F>
  inline
  typename thrubin_image<I1, I2, F>::rvalue
  thrubin_image<I1, I2, F>::operator()(const mln_psite(I1)& p)
  {
    mln_precondition(this->is_valid());
    return this->data_->f_(this->data_->ima_(p), this->data_->ima2_(p));
  }

  // thrubin
  template <typename I1, typename I2, typename F>
  thrubin_image<I1, I2, F>
  thru(const mln::Function<F>& f,
       Image<I1>& ima1, Image<I2>& ima2)
  {
    thrubin_image<I1, I2, F> tmp(exact(ima1), exact(ima2), exact(f).state());
    return tmp;
  }

  template <typename I1, typename I2, typename F>
  const thrubin_image<const I1, const I2, F>
  thru(const mln::Function<F>& f,
       const Image<I1>& ima1, const Image<I2>& ima2)
  {
    thrubin_image<const I1, const I2, F> tmp(exact(ima1), exact(ima2), exact(f).state());
    return tmp;
  }

  template <typename I1, typename I2, typename M>
  thrubin_image<I1, I2, mln_fun_withbin(M, mln_value(I1), mln_value(I2))>
  thru(const mln::Meta_Function<M>& f, Image<I1>& ima1, Image<I2>& ima2)
  {
    typedef mln_fun_withbin(M, mln_value(I1), mln_value(I2)) F;
    thrubin_image<I1, I2, F> tmp(exact(ima1), exact(ima2), F(exact(f).state()));

    return tmp;
  }

  template <typename I1, typename I2, typename M>
  const thrubin_image<const I1, const I2, mln_fun_withbin(M, mln_value(I1), mln_value(I2))>
  thru(const mln::Meta_Function<M>& f, const Image<I1>& ima1, const Image<I2>& ima2)
  {
    typedef mln_fun_withbin(M, mln_value(I1), mln_value(I2)) F;
    thrubin_image<const I1, const I2, F> tmp(exact(ima1), exact(ima2), F(exact(f).state()));

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VMORPH_THRUBIN_IMAGE_HH
