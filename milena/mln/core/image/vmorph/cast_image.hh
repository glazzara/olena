// Copyright (C) 2007, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_IMAGE_VMORPH_CAST_IMAGE_HH
# define MLN_CORE_IMAGE_VMORPH_CAST_IMAGE_HH

/// \file
///
/// Definition of an image morpher that make the user see the
/// same image but with another data type.
///
/// \todo Rename as cast_image (without '_')!

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/value/set.hh>
# include <mln/value/cast.hh>


namespace mln
{

  // Fwd decl.
  template <typename T, typename I> struct cast_image_;


  namespace internal
  {
    /*!
      \internal
      \brief Data structure for \c mln::cast_image_<T,I>.
    */
    template <typename T, typename I>
    struct data< cast_image_<T,I> >
    {
      data(const I& ima);
      const I& ima_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T, typename I, typename value_io>
    struct cast_image_trait_selector :
      default_image_morpher< I, T, cast_image_<T,I> >
    {
      typedef trait::image::vw_io::none vw_io;
      typedef trait::image::vw_set::none vw_set;
    };

    template <typename T, typename I>
    struct cast_image_trait_selector<T, I, trait::image::vw_io::read> :
      default_image_morpher< I, T, cast_image_<T,I> >
    {
      typedef trait::image::vw_io::read vw_io;
    };

    template <typename T, typename I>
    struct cast_image_trait_selector<T, I, trait::image::vw_io::read_write> :
      default_image_morpher< I, T, cast_image_<T,I> >
    {
      typedef trait::image::vw_io::read vw_io;
    };

    template <typename T, typename I>
    struct image_< cast_image_<T,I> > :
      cast_image_trait_selector<T, I, mln_trait_image_vw_io(I)>
    {
    private:
      typedef mlc_equal(mln_trait_value_quant(T),
			trait::value::quant::high) is_high_quant_;
    public:
      typedef mlc_if(is_high_quant_,
		     trait::image::quant::high,
		     trait::image::quant::low) quant;

      typedef trait::image::category::value_morpher category;
      typedef trait::image::value_io::read_only value_io;
      typedef trait::image::pw_io::read pw_io;
      typedef trait::image::value_access::indirect value_access;

      /// FIXME is the following property correct?
      typedef trait::image::ext_domain::none ext_domain;
    };

  } // end of namespace mln::trait



  /// \brief Class of image morpher which takes an image to change its
  /// data type.
  ///
  /// \ingroup modimagevaluemorpher
  template <typename T, typename I>
  struct cast_image_ :
    public internal::image_value_morpher< I, T, cast_image_<T,I> >
  {
    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef T rvalue;

    /// Return type of read-write access.
    typedef T lvalue;

    /// Skeleton.
    typedef cast_image_< tag::value_<T>, tag::image_<I> > skeleton;

    /// Constructor.
    cast_image_(const Image<I>& ima);

    /// Initialize an empty image.
    void init_(const Image<I>& ima);

    /// Read-only access of pixel value at point site \p p.
    T operator()(const mln_psite(I)& p) const;

    /// Mutable access is only OK for reading (not writing).
    T operator()(const mln_psite(I)& p);
  };


  template <typename T, typename I>
  cast_image_<T,I>
  cast_image(const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    cast_image_<T,I> tmp(ima);
    return tmp;
  }


# ifndef MLN_INCLUDE_ONLY


  // internal::data< cast_image_<T,I> >

  namespace internal
  {

    template <typename T, typename I>
    inline
    data< cast_image_<T,I> >::data(const I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal

  // cast_image_<T,I>

  template <typename T, typename I>
  inline
  cast_image_<T,I>::cast_image_(const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data< cast_image_<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  void
  cast_image_<T,I>::init_(const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data<cast_image_<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  T
  cast_image_<T,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->data_->ima_.has(p));
    return mln::value::cast<T>( this->data_->ima_(p) );
  }

  template <typename T, typename I>
  inline
  T
  cast_image_<T,I>::operator()(const mln_psite(I)& p)
  {
    return mln::value::cast<T>( this->data_->ima_(p) );
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VMORPH_CAST_IMAGE_HH
