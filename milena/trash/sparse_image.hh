// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_SPARSE_IMAGE_HH
# define MLN_CORE_IMAGE_SPARSE_IMAGE_HH

/*! \file mln/core/image/sparse_image.hh
 *
 * \brief Definition of an image with sparse encoding.
 */

# include <vector>
# include <mln/core/internal/image_primary.hh>
# include <mln/core/site_set/p_set_of.hh>
# include <mln/core/site_set/p_run.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Fwd decl.
  template <typename P, typename T> struct sparse_image;


  namespace internal
  {

    /// Data structure for \c mln::sparse_image<P,T>.
    template <typename P, typename T>
    struct data< sparse_image<P,T> >
    {
      data();
      data(const p_set_of< p_run<P> >& s);

      /// Domain.
      p_set_of< p_run<P> > domain_;

      /// Image.
      std::vector< std::vector<T> > values_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename P, typename T>
    struct image_< sparse_image<P,T> > : default_image_< T, sparse_image<P,T> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::value_access::direct       value_access;
      typedef trait::image::value_storage::piecewise   value_storage;
      typedef trait::image::value_browsing::value_wise value_browsing;
      typedef trait::image::value_io::read_write       value_io;

      // site / domain
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::two_d         dimension; // FIXME

      // extended domain
      typedef trait::image::ext_domain::none      ext_domain;
      typedef trait::image::ext_value::irrelevant ext_value;
      typedef trait::image::ext_io::irrelevant    ext_io;
    };

  } // end of namespace mln::trait



  /*! \brief RLE image with different value in runs.
   *
   *
   * Parameter \c P is the type of the image points.
   * Parameter \c T is the type of the pixel values.
   * This image is not point wise accessible.
   */
  template <typename P, typename T>
  class sparse_image
    : public internal::image_primary< P, p_set_of< p_run<P> >,
				      sparse_image<P,T> >
  {
    typedef sparse_image<P,T> self_;
    typedef internal::image_primary<P, p_set_of< p_run<P> >, self_> super_;
  public:

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;

    /// Skeleton.
    typedef sparse_image< tag::psite_<P>, tag::value_<T> > skeleton;


    /// Constructor without argument.
    sparse_image();

    /// Constructor from a set of runs.
    sparse_image(const p_set_of< p_run<P> >& s);

    /// Initialize an empty image.
    void init_(const p_set_of< p_run<P> >& s);

    /// Add a new range to the image.
    void insert(const p_run<P>& r, const std::vector<T>& vals);


    typedef typename super_::psite psite;

    /// Test if \p p is valid.
    bool has(const psite& p) const;

    /// Read-only access to the image value located at point \p p.
    rvalue operator()(const psite& p) const;

    /// Read-write access to the image value located at point \p p.
    lvalue operator()(const psite& p);

    /// Give the definition domain.
    const p_set_of< p_run<P> >& domain() const;
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< sparse_image<I,S> >
    template <typename P, typename T>
    inline
    data< sparse_image<P,T> >::data()
    {
    }

    template <typename P, typename T>
    inline
    data< sparse_image<P,T> >::data(const p_set_of< p_run<P> >& s)
    {
      mln_precondition(s.is_valid());
      this->domain_ = s;
      const unsigned nr = s.nelements();
      this->values_.resize(nr);
      for (unsigned r = 0; r < nr; ++r)
	this->values_[r].resize(s[r].nsites());
    }

  } // end of namespace mln::internal


  template <typename P, typename T>
  inline
  sparse_image<P,T>::sparse_image()
  {
  }

  template <typename P, typename T>
  inline
  sparse_image<P,T>::sparse_image(const p_set_of< p_run<P> >& s)
  {
    this->data_ = new internal::data< sparse_image<P,T> >(s);
  }

  template <typename P, typename T>
  inline
  void
  sparse_image<P,T>::init_(const p_set_of< p_run<P> >& s)
  {
    this->data_ = new internal::data< sparse_image<P,T> >(s);
  }


  template <typename P, typename T>
  inline
  void
  sparse_image<P,T>::insert(const p_run<P>& r, const std::vector<T>& vals)
  {
    mln_precondition(r.nsites() == vals.size());
    if (! this->is_valid())
      this->data_ = new internal::data< sparse_image<P,T> >();
    this->data_->domain_.insert(r);
    this->data_->values_.push_back(vals);
  }

  template <typename P, typename T>
  inline
  bool
  sparse_image<P,T>::has(const psite& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->domain_.has(p);
  }

  template <typename P, typename T>
  inline
  const T&
  sparse_image<P,T>::operator()(const psite& p) const
  {
    mln_precondition(this->has(p));
    return this->data_->values_[p.index()][p.p().index()];
  }

  template <typename P, typename T>
  inline
  T&
  sparse_image<P,T>::operator()(const psite& p)
  {
    mln_precondition(this->has(p));
    return this->data_->values_[p.index()][p.p().index()];
  }

  template <typename P, typename T>
  inline
  const p_set_of< p_run<P> >&
  sparse_image<P,T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->domain_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_SPARSE_IMAGE_HH
