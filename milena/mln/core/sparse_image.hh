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

#ifndef MLN_CORE_SPARSE_IMAGE_HH
# define MLN_CORE_SPARSE_IMAGE_HH

/*! \file mln/core/sparse_image.hh
 *
 * \brief Definition of mln::sparse_image
 */

# include <mln/core/internal/run_image.hh>
# include <mln/core/internal/run_pset.hh>
# include <mln/core/internal/run_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct sparse_image;

  namespace internal
  {

    template  <typename P, typename T>
    struct data_< sparse_image<P,T> >
    {
      data_();

      /// Image values.
      std::vector< std::vector<T> > values_;
      /// domain of the image
      run_pset_<P> domain_;
    };

  } // end of namespace mln::internal

  /*! \brief Sparse image.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class sparse_image : public internal::run_image_< P, sparse_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef internal::run_psite<P> psite;
    typedef mln::value::set<T> vset;
    typedef internal::run_pset_<P> pset;

    /// Skeleton.
    typedef sparse_image< tag::psite_<P>, tag::value_<T> > skeleton;


    sparse_image();

    /// Add a new range to the image.
    void insert(const P& p, unsigned len, const std::vector<T>& value);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const psite& p) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const psite& p);

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

    // internal::data_< sparse_image<I,S> >

    template <typename P, typename T>
    data_< sparse_image<P,T> >::data_()
    {
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  sparse_image<P, T>::sparse_image()
  {
    // FIXME : ambiguity between empty constructor and constructor
    // which allocate data_

    // this->data_ = new internal::data_< sparse_image<I,T> >();
  }

  template <typename P, typename T>
  bool
  sparse_image<P, T>::has_data() const
  {
    return this->data_->values_.size() != 0;
  }

  template <typename P, typename T>
  const typename sparse_image<P, T>::vset&
  sparse_image<P, T>::values() const
  {
    return vset::the();
  }

  template <typename P, typename T>
  void
  sparse_image<P, T>::insert(const P& p, unsigned len,
			     const std::vector<T>& value)
  {
    this->data_->domain_.insert(p, len);
    this->data_->values_.push_back(value);
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::rvalue
  sparse_image<P, T>::operator()
    (const typename sparse_image<P, T>::psite& site) const
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < this->data_->values_.size() &&
      site.index_() < this->data_->values_[site.pset_pos_()].size());
    return this->data_->values_[site.pset_pos_()][site.index_()];
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::lvalue
  sparse_image<P, T>::operator()
    (const typename sparse_image<P,T>::psite& site)
  {
    mln_precondition(this->has_data() &&
		     site.pset_pos_() < this->data_->values_.size() &&
		     site.index_() < this->data_->values_[site.pset_pos_()].size());
    return this->data_->values_[site.pset_pos_()][site.index_()];
  }

  template <typename P, typename T>
  const typename sparse_image<P, T>::pset&
  sparse_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_SPARSE_IMAGE_HH
