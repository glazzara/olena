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
 * \brief Definition of an image with rle encoding.
 */

# include <mln/core/internal/run_image.hh>
# include <mln/core/internal/run_pset.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct rle_image;


  namespace internal
  {

    template <typename P, typename T>
    struct data_< rle_image<P,T> >
    {
      data_();

      /// Image values.
      std::vector<T> values_;

      /// domain of the image
      run_pset_<P> domain_;
    };

  } // end of namespace mln::internal



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
    typedef internal::run_pset_<P> pset;


    /// Skeleton.
    typedef rle_image< tag::psite_<P>, tag::value_<T> > skeleton;


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

    /// Give the definition domain.
    const pset& domain() const;

  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< rle_image<I,S> >

    template <typename P, typename T>
    inline
    data_< rle_image<P,T> >::data_()
    {
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  inline
  rle_image<P, T>::rle_image()
  {
  }

  template <typename P, typename T>
  inline
  bool
  rle_image<P, T>::has_data() const
  {
    return this->data_->values_.size() != 0;
  }

  template <typename P, typename T>
  inline
  const typename rle_image<P, T>::vset&
  rle_image<P, T>::values() const
  {
    return vset::the();
  }

  template <typename P, typename T>
  inline
  void
  rle_image<P, T>::insert(const P& p, unsigned len, T value)
  {
    if (!this->has_data())
      this->data_ = new internal::data_< rle_image<P,T> >();
    this->data_->domain_.insert(p, len);
    this->data_->values_.push_back(value);
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::rvalue
  rle_image<P, T>::operator() (const typename rle_image<P, T>::psite& site)
    const
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < this->data_->values_.size());
    return this->data_->values_[site.pset_pos_()];
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::lvalue
  rle_image<P, T>::operator() (const typename rle_image<P, T>::psite& site)
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < this->data_->values_.size());
    return this->data_->values_[site.pset_pos_()];
  }

  template <typename P, typename T>
  inline
  const typename rle_image<P, T>::pset&
  rle_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_RLE_IMAGE_HH
