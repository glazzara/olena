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

#ifndef MLN_CORE_IMAGE_THRU_HH
# define MLN_CORE_IMAGE_THRU_HH

/*! \file mln/core/image/thru.hh
 *
 * \brief Definition of an image morpher that make the user see the
 * image thru a function
 *
 */

# include <mln/core/internal/image_value_morpher.hh>
# include <mln/trait/images.hh>
# include <mln/value/set.hh>
# include <mln/core/image/shell.hh>

namespace mln
{

  // Fwd decl.
  template <typename F, typename I> class thru;

  namespace internal
  {
    /// Data structure for \c mln::thru<T,I>.
    template <typename F, typename I>
    struct data< thru<F,I> >
    {
      data( I& ima);
      I& ima_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename F, typename I>
    struct image_< thru<F,I> > : default_image_morpher< I, mln_result(F), thru<F,I> >
    {
      typedef trait::image::value_io::read_write      value_io;
      typedef trait::image::value_access::computed    value_access;
      typedef trait::image::value_storage::disrupted  value_storage;
    };

  } // end of namespace mln::trait



  /*! \brief Class of image morpher which takes an image and a functor
   * FIXME
   */
  template <typename F, typename I>
  struct thru :
    public internal::image_value_morpher< I, mln_result(F), thru<F,I> >
  {
    /// Value associated type.
    typedef mln_result(F) value;

    /// Return type of read-only access.
    typedef mln_result(F) rvalue;

    /// Return type of read-write access.
    typedef shell<F,I> lvalue;

    /// Skeleton.
    typedef thru< tag::value_<mln_result(F)>, tag::image_<I> > skeleton;

    /// Constructor.
    thru(Image<I>& ima);

    /// Initialize an empty image.
    void init_(Image<I>& ima);

    /// Read-only access of pixel value at point site \p p.
    mln_result(F) operator()(const mln_psite(I)& p) const;

    /// Mutable access is for reading and writing.
    lvalue operator()(const mln_psite(I)& p);
  };

# ifndef MLN_INCLUDE_ONLY


  // internal::data< thru<T,I> >

  namespace internal
  {

    template <typename F, typename I>
    inline
    data< thru<F,I> >::data(I& ima)
      : ima_(ima)
    {
    }

  } // end of namespace mln::internal

  // thru<F,I>

  template <typename F, typename I>
  inline
  thru<F,I>::thru(Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    this->data_ = new internal::data< thru<F,I> >(exact(ima));
  }


  template <typename F, typename I>
  inline
  void
  thru<F,I>::init_(Image<I>& ima)
  {
    mln_precondition(exact(ima).has_data());
    this->data_ = new internal::data<thru<F,I> >(exact(ima));
  }


  template <typename F, typename I>
  inline
  mln_result(F)
  thru<F,I>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->data_->ima_.has(p));
    return F()( this->data_->ima_(p) );
  }

  template <typename F, typename I>
  inline
  shell<F, I>
  thru<F,I>::operator()(const mln_psite(I)& p)
  {
    return shell<F, I>( this->data_->ima_, p );
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_THRU_HH
