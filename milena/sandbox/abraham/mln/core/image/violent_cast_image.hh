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

#ifndef MLN_CORE_IMAGE_VIOLENT_CAST_IMAGE_HH
# define MLN_CORE_IMAGE_VIOLENT_CAST_IMAGE_HH

/*! \file mln/core/image/violent_cast_image.hh
 *
 * \brief Definition of an image morpher that make the user see the
 * same image but with another data type.
 *
 * \todo Rename as violent_cast_image (without '_')!
 */

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/value/set.hh>
# include <mln/value/cast.hh>


namespace mln
{

  // Fwd decl.
  template <typename T, typename I> class violent_cast_image_;

  namespace internal
  {
    /// Data structure for \c mln::violent_cast_image_<T,I>.
    template <typename T, typename I>
    struct data< violent_cast_image_<T,I> >
    {
      data(const I& ima);
      const I& ima_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T, typename I>
    struct image_< violent_cast_image_<T,I> > : default_image_morpher< I, T, violent_cast_image_<T,I> >
    {
      typedef trait::image::value_io::read_only value_io;
    };

  } // end of namespace mln::trait



  /*! \brief Class of image morpher which takes an image to change its
   *  data type.
   *
   */
  template <typename T, typename I>
  struct violent_cast_image_ :
    public internal::image_value_morpher< I, T, violent_cast_image_<T,I> >
  {
    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef T rvalue;

    /// Return type of read-write access.
    typedef T lvalue;

    /// Skeleton.
    typedef violent_cast_image_< tag::value_<T>, tag::image_<I> > skeleton;

    /// Constructor.
    violent_cast_image_(const Image<I>& ima);

    /// Initialize an empty image.
    void init_(const Image<I>& ima);

    /// Read-only access of pixel value at point site \p p.
    T operator()(const mln_psite(I)& p) const;

    /// Mutable access is only OK for reading (not writing).
    T operator()(const mln_psite(I)& p);
  };


  template <typename T, typename I>
  violent_cast_image_<T,I>
  violent_cast_image(const Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    violent_cast_image_<T,I> tmp(ima);
    return tmp;
  }


# ifndef MLN_INCLUDE_ONLY


  // internal::data< violent_cast_image_<T,I> >

  namespace internal
  {

    template <typename T, typename I>
    inline
    data< violent_cast_image_<T,I> >::data(const I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal

  // violent_cast_image_<T,I>

  template <typename T, typename I>
  inline
  violent_cast_image_<T,I>::violent_cast_image_(const Image<I>& ima)
  {
    metal::bool_<sizeof(T) == sizeof(typename I::value)>::check();
    mln_precondition(exact(ima).has_data());
    this->data_ = new internal::data< violent_cast_image_<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  void
  violent_cast_image_<T,I>::init_(const Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    this->data_ = new internal::data<violent_cast_image_<T,I> >(exact(ima));
  }

  template <typename T, typename I>
  inline
  T
  violent_cast_image_<T,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->data_->ima_.has(p));
    return *(T*)(void*)&( this->data_->ima_(p) );
  }

  template <typename T, typename I>
  inline
  T
  violent_cast_image_<T,I>::operator()(const mln_psite(I)& p)
  {
    return *(T*)(void*)&( this->data_->ima_(p) );
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_VIOLENT_CAST_IMAGE_HH
