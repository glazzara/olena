// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_IMORPH_PLAIN_HH
# define MLN_CORE_IMAGE_IMORPH_PLAIN_HH

/// \file
///
/// Definition of a morpher that prevents an image from sharing
/// his data.

# include <mln/core/internal/image_identity.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/metal/is_not_const.hh>


namespace mln
{

  // Forward declaration.
  template <typename I> class plain;


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for \c mln::plain<I>.
    */
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
      // ...these changes:
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
    };

  } // end of namespace mln::trait



  /// Prevents an image from sharing its data.
  ///
  /// While assigned to another image, its data is duplicated.
  ///
  /// \ingroup modimageidmorpher
  //
  template <typename I>
  class plain

    : public mln::internal::image_identity< I, mln_domain(I), plain<I> >,
      private mlc_is_not_const(I)::check_t
  {
    typedef plain<I> self_;
    typedef mln::internal::image_identity<I, mln_domain(I), self_> super_;

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
      : ima_(duplicate(ima))
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
    mln_precondition(rhs.is_valid());
    init_(rhs.data_->ima_);
  }

  template <typename I>
  inline
  plain<I>::plain(const I& ima)
  {
    mln_precondition(ima.is_valid());
    init_(ima);
  }

  template <typename I>
  inline
  void
  plain<I>::init_(const I& ima)
  {
    mln_precondition(ima.is_valid());
    this->data_ = new internal::data< plain<I> >(ima);
  }

  template <typename I>
  inline
  plain<I>&
  plain<I>::operator=(const plain<I>& rhs)
  {
    mln_precondition(rhs.is_valid());
    if (&rhs == this)
      return *this;
    this->destroy();
    init_(rhs.data_->ima_);
    return *this;
  }

  template <typename I>
  inline
  plain<I>&
  plain<I>::operator=(const I& ima)
  {
    mln_precondition(ima.is_valid());
    this->destroy();
    init_(ima);
    return *this;
  }

  template <typename I>
  inline
  plain<I>::operator I () const
  {
    mln_precondition(this->is_valid());
    return duplicate(this->data_->ima_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_IMORPH_PLAIN_HH
