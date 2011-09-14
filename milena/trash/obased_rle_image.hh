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

#ifndef MLN_CORE_IMAGE_OBASED_RLE_IMAGE_HH
# define MLN_CORE_IMAGE_OBASED_RLE_IMAGE_HH

/// \file mln/core/image/obased_rle_image.hh
///
/// Definition of an image with rle encoding.


# include <mln/core/internal/run_image.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/core/site_set/p_set_of.hh>
# include <mln/core/site_set/box.hh>
# include <mln/value/set.hh>
# include <vector>

namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct obased_rle_image;


  namespace internal
  {

    /// Data structure for \c mln::obased_rle_image<P,T>.
    template <typename P, typename T>
    struct data< obased_rle_image<P,T> >
    {
      data(const std::set<T>& values);

      /// Objects.
      std::vector< std::vector<unsigned> > obj_;

      /// Bounding boxes of the objects.
      std::vector< accu::shape::bbox<P> > bbox_;

      /// Value of Objects.
      std::vector<T> v_obj_;

      /// Image values.
      std::vector<T> values_;

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
    struct image_< obased_rle_image<P,T> > : default_image_< T, obased_rle_image<P,T> >
    {
      typedef trait::image::category::primary category;

      /// FIXME: Update with new properties!
/*      typedef trait::image::access::browsing   access;
      // FIXME: Put the right dimension.
      typedef trait::image::space::two_d     space;
      typedef trait::image::size::regular    size;
      typedef trait::image::support::aligned support;

      typedef trait::image::border::none     border;
      typedef trait::image::data::linear     data;
      typedef trait::image::io::read_only    io;
      typedef trait::image::speed::slow      speed;*/
    };

  } // end of namespace mln::trait


  /*! RLE image with objects.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class obased_rle_image : public internal::run_image_< T, P, obased_rle_image<P, T> >
  {
  public:
    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;
    typedef p_set_of< p_run<P> > pset;
    typedef mln_psite(pset) psite;


    /// Skeleton.
    typedef obased_rle_image< tag::psite_<P>, tag::value_<T> > skeleton;

    /// Constructor
    obased_rle_image(const std::set<T>& values);

    /// Initialize an empty image
    void init_(const std::set<T>& values);

    /// Add a new range to the image.
    void insert(const p_run<P>& pr, T value);

    /// Read-only access to the image value located at point \p p.
    rvalue operator() (const mln_psite(pset)& site) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator() (const mln_psite(pset)& site);

    /// Test if this image has been initialized.
    bool is_valid() const;

    /// Give the definition domain.
    const pset& domain() const;

    /// Give the index vector of the i-th object.
    const std::vector<unsigned>& object(unsigned i) const;

    /// Give the bounding box of the i-th object.
    const box<P>& bbox_of_run(unsigned i) const;

  };


# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< obased_rle_image<P,T> >

    template <typename P, typename T>
    inline
    data< obased_rle_image<P,T> >::data(const std::set<T>& values)
      : obj_(values.size()),
	bbox_(values.size())
    {
      std::copy(values.begin(), values.end(),
		std::back_inserter(this->v_obj_));
    }

    template <typename P, typename T>
    inline
    unsigned
    data< obased_rle_image<P,T> >::memory_size() const
    {
      return domain_.memory_size() + bbox_.size()
	* (sizeof(T) + sizeof(box<P>) + sizeof(std::vector<unsigned>))
	+ (sizeof(unsigned) + sizeof(T)) * domain_.nsites();
    }

    template <typename P, typename T>
    inline
    void
    data< obased_rle_image<P,T> >::finalize()
    {
      domain_.finalize();
    }

  } // end of namespace mln::internal

  template <typename P, typename T>
  inline
  obased_rle_image<P, T>::obased_rle_image(const std::set<T>& values)
  {
    init_(values);
  }

  template <typename P, typename T>
  inline
  void
  obased_rle_image<P, T>::init_(const std::set<T>& values)
  {
    this->data_ = new internal::data< obased_rle_image<P,T> >(values);
  }

  template <typename P, typename T>
  inline
  bool
  obased_rle_image<P, T>::is_valid() const
  {
    return this->data_->values_.size() != 0;
  }

  template <typename P, typename T>
  inline
  void
  obased_rle_image<P, T>::insert(const p_run<P>& pr, T value)
  {
    mln_assertion(this->data_->v_obj_.size() == 0 || this->data_->domain_.nsites() == 0 ||
		  pr[0] > this->data_->domain_[this->data_->domain_.nsites() - 1].start());
    this->data_->domain_.insert(pr);
    this->data_->values_.push_back(value);
    unsigned i;
    for (i = 0; i < this->data_->v_obj_.size()
	   && this->data_->v_obj_[i] != value; ++i)
      ;
    mln_assertion(i != this->data_->v_obj_.size());
    this->data_->obj_[i].push_back(this->data_->domain_.nsites() - 1);
    this->data_->bbox_[i].take(pr.bbox().pmin());
    this->data_->bbox_[i].take(pr.bbox().pmax());
  }

  template <typename P, typename T>
  inline
  typename obased_rle_image<P, T>::rvalue
  obased_rle_image<P, T>::operator() (const mln_psite(pset)& site)
    const
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  typename obased_rle_image<P, T>::lvalue
  obased_rle_image<P, T>::operator() (const mln_psite(pset)& site)
  {
    mln_precondition(this->has(site));
    return this->data_->values_[site.index()];
  }

  template <typename P, typename T>
  inline
  const typename obased_rle_image<P, T>::pset&
  obased_rle_image<P, T>::domain() const
  {
    return this->data_->domain_;
  }

  template <typename P, typename T>
  inline
  const std::vector<unsigned>&
  obased_rle_image<P, T>::object(unsigned i) const
  {
    mln_assertion(i < this->data_->obj_.size());
    return this->data_->obj_[i];
  }

  template <typename P, typename T>
  inline
  const box<P>&
  obased_rle_image<P, T>::bbox_of_run(unsigned i) const
  {
    mln_assertion(i < this->data_->bbox_.size());
    return this->data_->bbox_[i];
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_OBASED_RLE_IMAGE_HH
