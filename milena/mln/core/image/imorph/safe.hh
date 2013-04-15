// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMORPH_SAFE_HH
# define MLN_CORE_IMAGE_IMORPH_SAFE_HH

/// \file
///
/// \brief Definition of a morpher that makes image become accessible
/// at undefined location.
///
/// \todo Use 'instant' as the routine safe returns.

# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Forward declaration.
  template <typename I> class safe_image;


  namespace internal
  {

    /*!
      \brief Data structure for \c mln::safe_image<I>.
    */
    template <typename I>
    struct data< safe_image<I> >
    {
      data(I& ima, const mln_value(I)& default_value);

      I ima_;
      mln_value(I) default_value_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< safe_image<I> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
    };

  } // end of namespace mln::trait



  /// Makes an image accessible at undefined location.
  ///
  /// \ingroup modimageidmorpher
  //
  template <typename I>
  class safe_image : public internal::image_identity< I, mln_domain(I), safe_image<I> >
  {
  public:

    /// Skeleton.
    typedef safe_image< tag::image_<I> > skeleton;

    safe_image();
    safe_image(I& ima);
    safe_image(I& ima, const mln_value(I)& default_value);

    /// \cond INTERNAL_API
    // Initialize an empty image.
    void init_(I& ima, const mln_value(I)& default_value);
    /// \endcond

    mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);

    /// Const promotion via conversion.
    operator safe_image<const I>() const;
  };



  template <typename I>
  safe_image<I> safe(Image<I>& ima,
		     mln_value(I) default_value = mln_value(I)());

  template <typename I>
  safe_image<const I> safe(const Image<I>& ima,
			   mln_value(I) default_value = mln_value(I)());



# ifndef MLN_INCLUDE_ONLY

  // internal::data< safe_image<I,S> >

  namespace internal
  {

    template <typename I>
    inline
    data< safe_image<I> >::data(I& ima, const mln_value(I)& default_value)
      : ima_(ima),
	default_value_(default_value)
    {
    }

  } // end of namespace mln::internal

  // safe_image<I>

  template <typename I>
  inline
  safe_image<I>::safe_image()
  {
  }

  template <typename I>
  inline
  safe_image<I>::safe_image(I& ima, const mln_value(I)& default_value)
  {
    mln_precondition(ima.is_valid());
    init_(ima, default_value);
  }

  template <typename I>
  inline
  safe_image<I>::safe_image(I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima, mln_value(I)());
  }

  template <typename I>
  inline
  void
  safe_image<I>::init_(I& ima, const mln_value(I)& default_value)
  {
    mln_precondition(! this->is_valid());
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< safe_image<I> >(ima, default_value);
  }

  template <typename I>
  inline
  mln_rvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    if (! this->has(p))
      return this->data_->default_value_;
    return this->data_->ima_(p);
  }

  template <typename I>
  inline
  mln_morpher_lvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->is_valid());
    static mln_value(I) forget_it_;
    if (this->has(p))
      return this->data_->ima_(p);
    else
      // A copy of data_->default_value_ is returned.
      return forget_it_ = this->data_->default_value_;
  }

  template <typename I>
  inline
  safe_image<I>::operator safe_image<const I>() const
  {
    safe_image<const I> tmp(this->data_->ima_, this->data_->default_value_);
    return tmp;
  }

  // safe

  template <typename I>
  inline
  safe_image<I> safe(Image<I>& ima,
		     mln_value(I) default_value)
  {
    safe_image<I> tmp(exact(ima), default_value);
    return tmp;
  }

  template <typename I>
  inline
  safe_image<const I> safe(const Image<I>& ima,
			   mln_value(I) default_value)
  {
    safe_image<const I> tmp(exact(ima), default_value);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_SAFE_HH
