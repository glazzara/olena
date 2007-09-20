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

#ifndef MLN_CORE_CAST_IMAGE_HH
# define MLN_CORE_CAST_IMAGE_HH

/*! \file mln/core/cast_image.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/value/set.hh>
# include <mln/value/cast.hh>


namespace mln
{

  /*! \brief FIXME
   *
   */
  template <typename T, typename I>
  class cast_image_ : public internal::image_value_morpher_< I, cast_image_<T,I> >
  {
  public:

    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef T rvalue;

    /// Return type of read-write access.
    typedef T lvalue;

    /// Value set associated type.
    typedef mln::value::set<T> vset;


    /// Skeleton.
    typedef cast_image_< tag::value<T>, tag::image<I> > skeleton;


    /// Constructor.
    cast_image_(const Image<I>& ima);


    /// Read-only access of pixel value at point site \p p.
    T operator()(const mln_psite(I)& p) const;

    /// Mutable access is only OK for reading (not writing).
    T operator()(const mln_psite(I)& p);

    /// Give the set of values of the image.
    const vset& values() const;

    /// Access to delegatee pointer.
    const I* impl_delegatee_() const;

  protected:
    const I& ima_;
  };



  template <typename T, typename I>
  cast_image_<T,I>
  cast_image(const Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    cast_image_<T,I> tmp(ima);
    return tmp;
  }
  


# ifndef MLN_INCLUDE_ONLY

  template <typename T, typename I>
  cast_image_<T,I>::cast_image_(const Image<I>& ima)
    : ima_(exact(ima))
  {
    mln_precondition(exact(ima).has_data());
  }

  template <typename T, typename I>
  T
  cast_image_<T,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(ima_.owns_(p));
    return mln::value::cast<T>( ima_(p) );
  }

  template <typename T, typename I>
  T
  cast_image_<T,I>::operator()(const mln_psite(I)& p)
  {
    return mln::value::cast<T>( ima_(p) );
  }

  template <typename T, typename I>
  const mln::value::set<T>&
  cast_image_<T,I>::values() const
  {
    return vset::the();
  }

  template <typename T, typename I>
  const I*
  cast_image_<T,I>::impl_delegatee_() const
  {
    return & ima_;
  }
  
# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CAST_IMAGE_HH
