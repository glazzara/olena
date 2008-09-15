// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_PLAIN_HH
# define MLN_CORE_IMAGE_PLAIN_HH

/*! \file mln/core/image/plain.hh
 *
 * \brief Definition of a morpher that prevents an image from sharing
 * his data.
 */

# include <mln/core/internal/image_identity.hh>
# include <mln/core/routine/clone.hh>
# include <mln/metal/is_not_const.hh>


namespace mln
{

  // Forward declaration.
  template <typename I> struct plain;


  namespace internal
  {

    /// \internal Data structure for \c mln::plain<I>.
    template <typename I>
    struct data< plain<I> >
    {
      data(const I& ima);
      I ima_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I>
    struct image_< plain<I> > : image_< I > // Same as I except...
    {
      // ...this change.
      typedef trait::image::category::identity_morpher category;
    };

  } // end of namespace mln::trait



  /*! \brief FIXME
   *
   */
  template <typename I>
  class plain

    : public mln::internal::image_identity< I, mln_pset(I), plain<I> >,
      private mlc_is_not_const(I)::check_t
  {
    typedef plain<I> self_;
    typedef mln::internal::image_identity<I, mln_pset(I), self_> super_;

  public:

    /// Skeleton.
    typedef plain< tag::image_<I> > skeleton;

    /// Constructor without argument.
    plain();

    /// Copy constructor.
    plain(const plain<I>& rhs);

    /// Copy constructor from an image \p ima.
    plain(const I& ima);

    /// Initialize an empty image.
    void init_(const I& ima);

    /// Assignment operator.
    plain<I>& operator=(const plain<I>& rhs);

    /// Assignment operator from an image \p ima.
    plain<I>& operator=(const I& ima);

    /// Conversion into an image with type \c I.
    operator I () const;
  };



# ifndef MLN_INCLUDE_ONLY


  // internal::data< plain<I> >

  namespace internal
  {

    template <typename I>
    inline
    data< plain<I> >::data(const I& ima)
      : ima_(clone(ima))
    {
    }

  } // end of namespace mln::internal


  // plain<I>

  template <typename I>
  inline
  plain<I>::plain()
  {
  }

  template <typename I>
  inline
  plain<I>::plain(const plain<I>& rhs)
    : super_()
  {
    mln_precondition(rhs.has_data());
    init(rhs.data_->ima_);
  }

  template <typename I>
  inline
  plain<I>::plain(const I& ima)
  {
    mln_precondition(ima.has_data());
    init(ima);
  }

  template <typename I>
  inline
  void
  plain<I>::init_(const I& ima)
  {
    mln_precondition(ima.has_data());
    this->data_ = new internal::data< plain<I> >(ima);
  }

  template <typename I>
  inline
  plain<I>&
  plain<I>::operator=(const plain<I>& rhs)
  {
    mln_precondition(rhs.has_data());
    if (&rhs == this)
      return *this;
    this->destroy();
    init(rhs.data_->ima_);
    return *this;
  }

  template <typename I>
  inline
  plain<I>&
  plain<I>::operator=(const I& ima)
  {
    mln_precondition(ima.has_data());
    this->destroy();
    init(ima);
    return *this;
  }

  template <typename I>
  inline
  plain<I>::operator I () const
  {
    mln_precondition(this->has_data());
    return clone(this->data_->ima_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_PLAIN_HH
