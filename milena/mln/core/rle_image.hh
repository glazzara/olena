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

#ifndef MLN_CORE_RLE_IMAGE_HH
# define MLN_CORE_RLE_IMAGE_HH

/*! \file mln/core/rle_image.hh
 *
 * \brief Definition of mln::rle_image
 */

# include <mln/core/internal/run_image.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  /*! \brief RLE image.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class rle_image : public internal::run_image_< P, rle_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef internal::run_psite<P> psite;
    typedef mln::value::set<T> vset;


    /// Skeleton.
    typedef rle_image< tag::psite<P>, tag::value<T> > skeleton;


    rle_image();

    /// Add a new range to the image.
    void insert(const P& p, unsigned len, T value);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const psite& site) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const psite& site);

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Give the set of values of the image.
    const vset& values() const;

  protected:
    /// Image values.
    std::vector<T> values_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename T>
  rle_image<P, T>::rle_image()
  {
  }

  template <typename P, typename T>
  bool
  rle_image<P, T>::has_data() const
  {
    return values_.size() != 0;
  }

  template <typename P, typename T>
  const typename rle_image<P, T>::vset&
  rle_image<P, T>::values() const
  {
    return vset::the();
  }

  template <typename P, typename T>
  void
  rle_image<P, T>::insert(const P& p, unsigned len, T value)
  {
    this->domain_.insert(p, len);
    values_.push_back(value);
  }

  template <typename P, typename T>
  typename rle_image<P, T>::rvalue
  rle_image<P, T>::operator() (const typename rle_image<P, T>::psite& site)
    const
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < values_.size());
    return values_[site.pset_pos_()];
  }

  template <typename P, typename T>
  typename rle_image<P, T>::lvalue
  rle_image<P, T>::operator() (const typename rle_image<P, T>::psite& site)
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < values_.size());
    return values_[site.pset_pos_()];
  }
# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_RLE_IMAGE_HH
