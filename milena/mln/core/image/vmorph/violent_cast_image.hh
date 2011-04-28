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

#ifndef MLN_CORE_IMAGE_VMORPH_VIOLENT_CAST_IMAGE_HH
# define MLN_CORE_IMAGE_VMORPH_VIOLENT_CAST_IMAGE_HH

/// \file
///
/// definition of an image morpher that make the user see the
/// same image but with another data type.

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/value/set.hh>
# include <mln/value/cast.hh>


namespace mln
{

  // Fwd decl.
  template <typename T, typename I> struct violent_cast_image;

  namespace internal
  {
    /// Data structure for \c mln::violent_cast_image<T,I>.
    template <typename T, typename I>
    struct data< violent_cast_image<T,I> >
    {
      data(const I& ima);
      const I& ima_;
    };

  } // end of namespace mln::internal



  namespace trait
  {
    template <typename T, typename I, typename value_io>
    struct violent_cast_image_trait_selector :
      default_image_morpher< I, T, violent_cast_image<T,I> >
    {
      typedef trait::image::vw_io::none vw_io;
      typedef trait::image::vw_set::none vw_set;
    };

    template <typename T, typename I>
    struct violent_cast_image_trait_selector<T, I, trait::image::vw_io::read> :
      default_image_morpher< I, T, violent_cast_image<T,I> >
    {
      typedef trait::image::vw_io::read vw_io;
    };

    template <typename T, typename I>
    struct violent_cast_image_trait_selector<T, I, trait::image::vw_io::read_write> :
      default_image_morpher< I, T, violent_cast_image<T,I> >
    {
      typedef trait::image::vw_io::read vw_io;
    };

    template <typename T, typename I>
    struct image_< violent_cast_image<T,I> > :
      violent_cast_image_trait_selector<T, I, mln_trait_image_vw_io(I)>
    {
    private:
      typedef mlc_equal(mln_trait_value_quant(T),
			trait::value::quant::high) is_high_quant_;
    public:
      typedef mlc_if(is_high_quant_,
		     trait::image::quant::high,
		     trait::image::quant::low) quant;

      typedef trait::image::value_io::read_only value_io;
      typedef trait::image::pw_io::read pw_io;
      typedef trait::image::value_access::indirect value_access;
      typedef trait::image::category::value_morpher category;

      /// FIXME is the following property correct?
      typedef trait::image::ext_domain::none ext_domain;
    };
 } // end of namespace mln::trait



  /// Violently cast image values to a given type.
  ///
  /// \ingroup modimagevaluemorpher
  //
  template <typename T, typename I>
  struct violent_cast_image :
    public internal::image_value_morpher< I, T, violent_cast_image<T,I> >
  {
    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef T rvalue;

    /// Return type of read-write access.
    typedef T lvalue;

    /// Skeleton.
    typedef violent_cast_image< tag::value_<T>, tag::image_<I> > skeleton;

    /// Constructor.
    violent_cast_image(const Image<I>& ima);

    /// Initialize an empty image.
    void init_(const Image<I>& ima);

    /// Read-only access of pixel value at point site \p p.
    T operator()(const mln_psite(I)& p) const;

    /// Mutable access is only OK for reading (not writing).
    T operator()(const mln_psite(I)& p);
  };


  template <typename T, typename I>
  violent_cast_image<T,I>
  violent_cast_image_(const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    violent_cast_image<T,I> tmp(ima);
    return tmp;
  }


# ifndef MLN_INCLUDE_ONLY


  // internal::data< violent_cast_image<T,I> >

  namespace internal
  {

    template <typename T, typename I>
    inline
    data< violent_cast_image<T,I> >::data(const I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal

  // violent_cast_image<T,I>

  template <typename T, typename I>
  inline
  violent_cast_image<T,I>::violent_cast_image(const Image<I>& ima)
  {
    metal::bool_<sizeof(T) == sizeof(typename I::value)>::check();
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data< violent_cast_image<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  void
  violent_cast_image<T,I>::init_(const Image<I>& ima)
  {
    mln_precondition(exact(ima).is_valid());
    this->data_ = new internal::data<violent_cast_image<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  T
  violent_cast_image<T,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->data_->ima_.has(p));
    return *(T*)(void*)&( this->data_->ima_(p) );
  }

  template <typename T, typename I>
  inline
  T
  violent_cast_image<T,I>::operator()(const mln_psite(I)& p)
  {
    return *(T*)(void*)&( this->data_->ima_(p) );
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VMORPH_VIOLENT_CAST_IMAGE_HH
