// Copyright (C) 2007,2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_MONO_RLE_IMAGE_HH
# define MLN_CORE_IMAGE_MONO_RLE_IMAGE_HH

/// \file mln/core/image/mono_rle_image.hh
///
/// Definition of an image with rle encoding.

# include <mln/core/internal/run_image.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/core/site_set/p_set_of.hh>
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
      p_set_of< p_run<P> > domain_;

      /// Return the size of the data in memory.
      unsigned memory_size() const;

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

      // FIXME: Update with new properties!

//      typedef trait::image::access::browsing   access;
      // FIXME: Put the right dimension.
//      typedef trait::image::space::two_d     space;
//      typedef trait::image::size::regular    size;
//      typedef trait::image::support::aligned support;
//
//      typedef trait::image::border::none     border;
//      typedef trait::image::data::linear     data;
//      typedef trait::image::io::read_only    io;
//      typedef trait::image::speed::slow      speed;

    };

  } // end of namespace mln::trait


  /*! Monochrome RLE image.
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
    typedef p_set_of< p_run<P> > pset;
    typedef mln_psite(pset) psite;


    /// Skeleton.
    typedef mono_rle_image< tag::psite_<P>, tag::value_<T> > skeleton;


    /// Constructor
    mono_rle_image(const T& val);

    /// Initialize an empty image.
    void init_(const T& val);

    /// Add a new range to the image.
    void insert(const p_run<P>& pr);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const mln_psite(pset)& site) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const mln_psite(pset)& site);

    /// Test if this image has been initialized.
    bool is_valid() const;

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
    data< mono_rle_image<P,T> >::memory_size() const
    {
      return sizeof(T) + domain_.memory_size();
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
    init_(val);
  }

  template <typename P, typename T>
  inline
  void
  mono_rle_image<P, T>::init_(const T& val)
  {
    this->data_ = new internal::data< mono_rle_image<P,T> >(val);
  }

  template <typename P, typename T>
  inline
  bool
  mono_rle_image<P, T>::is_valid() const
  {
    return true;
  }

  template <typename P, typename T>
  inline
  void
  mono_rle_image<P, T>::insert(const p_run<P>& pr)
  {
    if (this->data_->domain_.nsites() != 0)
      mln_assertion(pr[0] > this->data_->domain_[this->data_->domain_.nsites() - 1].start());
    this->data_->domain_.insert(pr);
  }

  template <typename P, typename T>
  inline
  typename mono_rle_image<P, T>::rvalue
  mono_rle_image<P, T>::operator() (const mln_psite(pset)& site) const
  {
    mln_precondition(this->has(site));
    return this->data_->value_;
  }

  template <typename P, typename T>
  inline
  typename mono_rle_image<P, T>::lvalue
  mono_rle_image<P, T>::operator() (const mln_psite(pset)& site)
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
