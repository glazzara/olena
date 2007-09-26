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

#ifndef MLN_CORE_TR_IMAGE_HH
# define MLN_CORE_TR_IMAGE_HH

/*! \file mln/core/tr_image.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/metal/vec.hh>


namespace mln
{

  // Fwd decl.
  template <typename T, typename I> struct tr_image;

  namespace internal
  {

    template <typename T, typename I>
    struct data_< tr_image<T,I> >
    {
      data_(I& ima, T& tr);

      I ima_;
      T tr_;
    };

  } // end of namespace mln::internal

  /*! \brief FIXME
   *
   */
  template <typename T, typename I>
  struct tr_image : public mln::internal::image_identity_< I, mln_pset(I), tr_image<T,I> >
  {

    /// Super type.
    typedef mln::internal::image_identity_< I, mln_pset(I), tr_image<T,I> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Return type of read-write access.
    typedef mln_lvalue(I) lvalue; // FIXME: Depends on lvalue presence in I.

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Value set associated type.
    typedef mln::value::set<value> vset;


    /// Skeleton.
    typedef tr_image< T, tag::image_<I> > skeleton;


    /// Constructors.
    tr_image(I& ima, T& tr);
    tr_image();


    /// Test if this image has been initialized.
    bool has_data() const;

    /// Test if a pixel value is accessible at \p p.
    using super_::owns_;

    /// Test if a pixel value is accessible at \p v.
    bool owns_(const mln::metal::vec<I::point::dim, float>& v) const;

    /// Read-only access of pixel value at point site \p p.
    /// Mutable access is only OK for reading (not writing).
    using super_::operator();

    mln_value(I) operator()(const mln::metal::vec<I::point::dim, float>& v) const;

    void set_tr(T& tr);
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< tr_image<I,S> >

    template <typename T, typename I>
    data_< tr_image<T,I> >::data_(I& ima, T& tr)
      : ima_(ima),
	tr_(tr)
    {
    }

  } // end of namespace mln::internal

  template <typename T, typename I>
  tr_image<T,I>::tr_image(I& ima, T& tr)
  {
    mln_precondition(ima.has_data());
    this->data_ = new internal::data_< tr_image<T,I> >(ima, tr);
  }

  template <typename T, typename I>
  tr_image<T,I>::tr_image()
  {
  }

  template <typename T, typename I>
  bool tr_image<T,I>::has_data() const
  {
    mln_invariant(this->data_->ima_.has_data());
    return true;
  }

  template <typename T, typename I>
  bool tr_image<T,I>::owns_(const metal::vec<I::point::dim, float>& v) const
  {
    mln_point(I) p;
    metal::vec<I::point::dim, float> v2 = this->data_->tr_.inv()(v);
    for (unsigned i = 0; i < I::point::dim; ++i)
      p[i] = static_cast<int>(round(v2[i]));
    return this->data_->ima_.owns_(p);
  }

  template <typename T, typename I>
  mln_value(I)
  tr_image<T,I>::operator()(const metal::vec<I::point::dim, float>& v) const
  {
    mln_point(I) p;
    metal::vec<I::point::dim, float> v2 = this->data_->tr_.inv()(v);
    for (unsigned i = 0; i < I::point::dim; ++i)
      p[i] = static_cast<int>(round(v2[i]));
    mln_assertion(this->data_->ima_.owns_(p));
    return this->data_->ima_(p);
  }

  template <typename T, typename I>
  void
  tr_image<T,I>::set_tr(T& tr)
  {
    this->data_->tr_ = tr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_TR_IMAGE_HH
