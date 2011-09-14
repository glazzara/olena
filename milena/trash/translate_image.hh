// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_TRANSLATE_IMAGE_HH
# define MLN_CORE_IMAGE_TRANSLATE_IMAGE_HH

/// \file mln/core/image/translate_image.hh
///
/// Definition of an image which be translated by a delta point.

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/core/alias/box2d.hh>

//FIXME: Fix this image morpher.

namespace mln
{

  // Fwd decl.
  template <typename I> struct translate_image;

  namespace internal
  {

    /// Data structure for \c mln::translate_image<I>.
    template <typename I>
    struct data< translate_image<I> >
    {
      data(I& ima, const mln_site(I)::dpsite dp);

      I			    ima_;

      /// Real domain.
      box2d		    bb_;

      /// Delta point of translation.
      const mln_site(I)::dpsite   dp_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I>
    struct image_< translate_image<I> > : default_image_morpher< I, mln_value(I),
								  translate_image<I> >
    {
      typedef trait::image::category::domain_morpher category;

      typedef mln_trait_image_value_access(I)     value_access;
      typedef mln_trait_image_localization(I)     localization;
      typedef mln_trait_image_size(I)		  size;
      //typedef mln_trait_image_io_from_(I)   io;
      //typedef mln_trait_image_data_from_(I) data;
    };

  } // end of namespace mln::trait



  /// \brief Translate image class.
  ///
  /// The parameter \c I is the type of image.  This image class
  /// makes a translation of a delta point.
  ///
  template <typename I>
  struct translate_image : public mln::internal::image_identity< I, mln_pset(I), translate_image<I> >
  {

    typedef mln::internal::image_identity< I, mln_pset(I), translate_image<I> > super_;

    /// Return type of read-write access.
    typedef mln_morpher_lvalue(I) lvalue;

    /// Skeleton.
    typedef translate_image< tag::image_<I> > skeleton;

    /// Test if a pixel value is accessible at \p p.
    using super_::is_valid;

    /// Constructors.
    translate_image(I& ima, const mln_site(I)::dpsite dp);
    translate_image();

    /// Return domain of translated_image.
    const box2d& domain() const;

    /// Test if a pixel value is accessible at \p p.
    bool has(const mln_psite(I)& ) const;

    /// Read-only access of pixel value at point site \p p.
    mln_rvalue(I) operator()(const mln_psite(I)& p) const;

    /// Read and "write if possible" access of pixel value at point site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< translate_image<I,S> >

    template <typename I>
    inline
    data< translate_image<I> >::data(I& ima, const mln_site(I)::dpsite dp)
      : ima_ (ima),
	dp_ (dp)
    {
      point2d start = ima.bbox ().pmin () + dp;
      point2d end = ima.bbox ().pmax () + dp;
      int x1 = start[0];
      int y1 = start[1];
      int x2 = end[0];
      int y2 = end[1];

      bb_ = make::box2d (x1, y1, x2, y2);
    }

  } // end of namespace mln::internal

  template <typename I>
  inline
  translate_image<I>::translate_image(I& ima, const mln_site(I)::dpsite dp)
  {
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< translate_image<I> >(ima, dp);
  }

  template <typename I>
  inline
  translate_image<I>::translate_image()
  {
  }

  template <typename I>
  inline
  bool translate_image<I>::has(const mln_psite(I)& p) const
  {
    mln_psite(I) np = p - this->data_->dp_;
    return this->data_->ima_.has(np);
  }

  template <typename I>
  inline
  mln_rvalue(I)
  translate_image<I>::operator()(const mln_psite(I)& p) const
  {
    mln_assertion(this->has(p));
    mln_psite(I) np = p - this->data_->dp_;
    return this->data_->ima_(np);
  }


  template <typename I>
  inline
  mln_morpher_lvalue(I)
  translate_image<I>::operator()(const mln_psite(I)& p)
  {
    mln_assertion(this->has(p));
    mln_psite(I) np = p - this->data_->dp_;
    return this->data_->ima_(np);
  }

  template <typename I>
  inline
  const box2d&
  translate_image<I>::domain() const
  {
    return this->data_->bb_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_TRANSLATE_IMAGE_HH
