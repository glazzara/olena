// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_FLAT_IMAGE_HH
# define MLN_CORE_FLAT_IMAGE_HH

/*!
 * \file mln/core/image/flat_image.hh
 *
 * \brief Definition of a image with a signle value.
 *
 * \todo Zed: Address the values v. destination issue.
 */

# include <mln/core/internal/image_primary.hh>
# include <mln/value/set.hh>


namespace mln
{


  // Forward declaration.
  template <typename T, typename S> class flat_image;


  namespace internal
  {

    /// Data structure for \c mln::flat_image<T,S>.
    template <typename T, typename S>
    struct data< flat_image<T,S> >
    {
      data(const T& val, const S& pset);

      T val_;
      S pset_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename T, typename S>
    struct image_< flat_image<T,S> > : default_image_< T, flat_image<T,S> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::fast       speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::read_write               vw_io;
      typedef trait::image::vw_set::none                    vw_set;
      typedef trait::image::value_access::direct            value_access;
      typedef trait::image::value_storage::singleton        value_storage;
      typedef mln::trait::image::value_browsing::value_wise value_browsing;
      typedef trait::image::value_alignement::with_grid     value_alignement;
      typedef trait::image::value_io::read_only             value_io;

      // site / domain
      typedef trait::image::pw_io::read              pw_io;
      typedef trait::image::localization::basic_grid localization; // FIXME
      typedef trait::image::dimension::two_d         dimension;    // FIXME

      // extended domain
      typedef trait::image::ext_domain::infinite ext_domain;
      typedef trait::image::ext_value::single    ext_value;
      typedef trait::image::ext_io::read_only    ext_io;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename T, typename S>
  struct flat_image : public internal::image_primary< T, S, flat_image<T,S> >
  {
    /// Skeleton.
    typedef flat_image< tag::value_<T>, tag::pset_<S> > skeleton;


    /// Value associated type.
    typedef T value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef const T& lvalue;


    /// Constructor without argument.
    flat_image();

    /// Constructor.
    flat_image(const T& val, const S& pset);

    /// Initialization.
    void init_(const T& val, const S& pset);

    /// Give the definition domain.
    const S& domain() const;

    /// Test if \p p is valid: always return true.
    bool has(const mln_psite(S)& p) const;


    /// Read-only access to the image value located at point \p p.
    const T& operator()(const mln_psite(S)& p) const;

    /// Read-write access to the image value located at point \p p.
    const T& operator()(const mln_psite(S)& p);


    /// Change the image value.
    void change_value(const T& old_val, const T& new_val);
    const T& val() const;
    T& val();
  };



  template <typename T, typename S, typename J>
  void init_(tag::image_t, flat_image<T,S>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T, typename S, typename J>
  inline
  void init_(tag::image_t, flat_image<T,S>& target, const J& model)
  {
    S pset;
    init_(tag::domain, pset, model);
    T dummy;
    target.init_(dummy, pset);
  }


  // internal::data< flat_image<T,S> >

  namespace internal
  {

    template <typename T, typename S>
    inline
    data< flat_image<T,S> >::data(const T& val, const S& pset)
      : val_(val),
	pset_(pset)
    {
    }

  } // end of namespace mln::internal


  // flat_image<T,S>

  template <typename T, typename S>
  inline
  flat_image<T,S>::flat_image()
  {
  }

  template <typename T, typename S>
  inline
  flat_image<T,S>::flat_image(const T& val, const S& pset)
  {
    init_(val, pset);
  }

  template <typename T, typename S>
  inline
  void
  flat_image<T,S>::init_(const T& val, const S& pset)
  {
    mln_precondition(! this->has_data());
    this->data_ = new internal::data< flat_image<T,S> >(val, pset);
  }

  template <typename T, typename S>
  inline
  const S&
  flat_image<T,S>::domain() const
  {
    return this->data_->pset_;
  }

  template <typename T, typename S>
  inline
  bool
  flat_image<T,S>::has(const mln_psite(S)&) const
  {
    return true;
  }

  template <typename T, typename S>
  inline
  const T&
  flat_image<T,S>::operator()(const mln_psite(S)&) const
  {
    mln_precondition(this->has_data());
    return this->data_->val_;
  }

  template <typename T, typename S>
  inline
  const T&
  flat_image<T,S>::operator()(const mln_psite(S)&)
  {
    mln_precondition(this->has_data());
    return this->data_->val_;
  }

  template <typename T, typename S>
  inline
  void
  flat_image<T,S>::change_value(const T& old_val, const T& new_val)
  {
    mln_precondition(this->has_data());
    mln_precondition(old_val == this->data_->val_);
    this->data_->val_ = new_val;

    /// Avoid warning when NDEBUG is set
    (void) old_val;
  }

  template <typename T, typename S>
  inline
  const T&
  flat_image<T,S>::val() const
  {
    mln_precondition(this->has_data());
    return this->data_->val_;
  }

  template <typename T, typename S>
  inline
  T&
  flat_image<T,S>::val()
  {
    mln_precondition(this->has_data());
    return this->data_->val_;
  }



# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_FLAT_IMAGE_HH
