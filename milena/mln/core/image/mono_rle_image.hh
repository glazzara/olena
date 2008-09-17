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

#ifndef MLN_CORE_IMAGE_MONO_RLE_IMAGE_HH
# define MLN_CORE_IMAGE_MONO_RLE_IMAGE_HH

/*! \file mln/core/image/mono_rle_image.hh
 *
 * \brief Definition of an image with rle encoding.
 */

# include <mln/core/internal/run_image.hh>
# include <mln/core/p_runs.hh>
# include <mln/core/runs_psite.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct mono_rle_image;


  namespace internal
  {

    /// Data structure for \c mln::mono_rle_image<P,T>.
    template <typename P, typename T>
    struct data< mono_rle_image<P,T> >
    {
      data(const T& val);

      /// Image value.
      T value_;

      /// domain of the image
      p_runs_<P> domain_;

      /// Return the size of the data in memory.
      unsigned size_mem() const;

      /// Finalize the domain (internal use).
      void finalize();
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename T>
    struct image_< mono_rle_image<P,T> > : default_image_< T, mono_rle_image<P,T> >
    {
      typedef trait::image::category::primary category;

      typedef trait::image::access::browsing   access;
      // FIXME: Put the right dimension.
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none     border;
      typedef trait::image::data::linear     data;
      typedef trait::image::io::read_only    io;
      typedef trait::image::speed::slow      speed;
    };

  } // end of namespace mln::trait


  /*! \brief Monochrome RLE image.
   *
   * RLE image with only one colour.
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class mono_rle_image : public internal::run_image_< T, P, mono_rle_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef runs_psite<P> psite;
    typedef p_runs_<P> pset;


    /// Skeleton.
    typedef mono_rle_image< tag::psite_<P>, tag::value_<T> > skeleton;


    mono_rle_image(const T& val);

    /// Add a new range to the image.
    void insert(const p_run<P>& pr);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const runs_psite<P>& site) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const runs_psite<P>& site);

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Give the uniq value of the image.
    rvalue get_value() const;

    /// Give the definition domain.
    const pset& domain() const;

  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< mono_rle_image<I,S> >

    template <typename P, typename T>
    inline
    data< mono_rle_image<P,T> >::data(const T& val)
      : value_(val)
    {
    }

    template <typename P, typename T>
    inline
    unsigned
    data< mono_rle_image<P,T> >::size_mem() const
    {
      return sizeof(T) + domain_.size_mem();
    }

    template <typename P, typename T>
    inline
    void
    data< mono_rle_image<P,T> >::finalize()
    {
      domain_.finalize();
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  inline
  mono_rle_image<P, T>::mono_rle_image(const T& val)
  {
    this->data_ = new internal::data< mono_rle_image<P,T> >(val);
  }

  template <typename P, typename T>
  inline
  bool
  mono_rle_image<P, T>::has_data() const
  {
    return true;
  }

  template <typename P, typename T>
  inline
  void
  mono_rle_image<P, T>::insert(const p_run<P>& pr)
  {
    if (this->data_->domain_.nruns() != 0)
      mln_assertion(pr.first() > this->data_->domain_[this->data_->domain_.nruns() - 1].first());
    this->data_->domain_.insert(pr);
  }

  template <typename P, typename T>
  inline
  typename mono_rle_image<P, T>::rvalue
  mono_rle_image<P, T>::operator() (const runs_psite<P>& site) const
  {
    mln_precondition(this->has(site));
    return this->data_->value_;
  }

  template <typename P, typename T>
  inline
  typename mono_rle_image<P, T>::lvalue
  mono_rle_image<P, T>::operator() (const runs_psite<P>& site)
  {
    mln_precondition(this->has(site));
    return this->data_->value_;
  }

  template <typename P, typename T>
  inline
  typename mono_rle_image<P, T>::rvalue
  mono_rle_image<P, T>::get_value() const
  {
    return this->data_->value_;
  }

  template <typename P, typename T>
  inline
  const typename mono_rle_image<P, T>::pset&
  mono_rle_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_MONO_RLE_IMAGE_HH
