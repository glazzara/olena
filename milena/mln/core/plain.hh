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

#ifndef MLN_CORE_PLAIN_HH
# define MLN_CORE_PLAIN_HH

/*! \file mln/core/plain.hh
 *
 * \brief Definition of a morpher that prevent an image from sharing
 * his data.
 */

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/core/clone.hh>

# include <mln/algebra/vec.hh>


namespace mln
{

  // Fwd decl.
  template <typename I> struct plain;

  namespace internal
  {

    /// \internal Data structure for \c mln::plain<I>.
    template <typename I>
    struct data_< plain<I> >
    {
      data_(const I& ima);

      I ima_;
    };

  } // end of namespace mln::internal

  /*! \brief FIXME
   *
   */
  template <typename I>
  struct plain : public mln::internal::image_identity< I, mln_pset(I), plain<I> >
  {

    typedef mln::internal::image_identity< I, mln_pset(I), plain<I> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Return type of read-write access.
    typedef mln_lvalue(I) lvalue; // FIXME: Depends on lvalue presence in I.

    /// Return type of read-only access.
    typedef mln_rvalue(I) rvalue;

    /// Skeleton.
    typedef plain< tag::image_<I> > skeleton;


    /// Constructors.
    plain(const I& ima);
    plain();

    /// Read-only access of pixel value at point site \p p.
    /// Mutable access is only OK for reading (not writing).
    //using super_::operator();

    /// Assignment operator.
    plain& operator=(const I& rhs);


    /// Conversion into an I image
    operator I () const;
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data_< plain<I> >

    template <typename I>
    inline
    data_< plain<I> >::data_(const I& ima)
      : ima_(clone(ima))
    {
    }

  } // end of namespace mln::internal

  template <typename I>
  inline
  plain<I>::plain(const I& ima)
  {
    mln_precondition(ima.has_data());
    this->data_ = new internal::data_< plain<I> >(ima);
  }


  template <typename I>
  inline
  plain<I>::plain()
  {
  }

  template <typename I>
  inline
  plain<I>&
  plain<I>::operator=(const I& rhs)
  {
    mln_precondition(rhs.has_data());
    this->destroy();
    this->data_ = new internal::data_< plain<I> >(rhs);
    return *this;
  }

  template <typename I>
  inline
  plain<I>::operator I () const
  {
    return clone(this->data_->ima_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PLAIN_HH
