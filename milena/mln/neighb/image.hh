// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_NEIGHB_IMAGE_HH
# define MLN_NEIGHB_IMAGE_HH

/// \file mln/neighb/image.hh
/// \brief An image morpher adding a neighborhood to an image.

# include <mln/core/internal/image_identity.hh>
# include <mln/core/concept/neighborhood.hh>


namespace mln
{

  // Fwd decl.
  namespace neighb { template <typename I, typename N> struct image; }

  /// Create an image with a neighborhood from an image and a
  /// neighborhood.
  template <typename I, typename N>
  neighb::image<I, N>
  operator+ (Image<I>& ima, const Neighborhood<N>& nbh);


  namespace internal
  {

    /// \internal Data structure for mln::neighb::image
    template <typename I, typename N>
    struct data_< mln::neighb::image<I, N> >
    {
      data_(I& ima, const N& nbh);

      I ima_;
      N nbh_;
    };

  } // end of namespace mln::internal



  namespace trait
  {

    template <typename I, typename N>
    struct image_< neighb::image<I, N> >
      : public default_image_morpher_< I, mln_value(I), neighb::image<I, N> >
      
    {
      typedef trait::image::category::morpher category;

      typedef trait::image::neighb::some      neighb;
    };

  } // end of namespace mln::trait


  namespace neighb
  {
    /// \brief A class that embodies an image with an attached
    /// neighborhood.
    /// 
    /// Parameter \c I is the actual type of the image, and \c N is
    /// the one of the neighborhood.
    ///
    /// \pre The domains of \c I and \c N must be compatible.
    template <typename I, typename N>
    struct image
      : public internal::image_identity_ < I, mln_pset(I), image <I, N> >
    {
      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Point_Set associated type.
      typedef mln_pset(I) pset;

      /// Neighborhood associated type.
      typedef N neighb;

      /// Skeleton.
      typedef image< tag::image_<I>, tag::neighb_<N> > skeleton;

      /// Constructor.
      image(Image<I>& ima, const Neighborhood<N>& nbh);

      /// Test if this image has been initialized.
      bool has_data() const;

      /// Return the neighborhood associated to this image.
      const neighb& neighborhood() const;
    }; 

  } // end of namespace mln::neighb


# ifndef MLN_INCLUDE_ONLY

  /*------------.
  | operator+.  |
  `------------*/

  template <typename I, typename N>
  neighb::image<I, N>
  operator+ (Image<I>& ima, const Neighborhood<N>& nbh)
  {
    neighb::image<I, N> tmp(ima, nbh);
    return tmp;
  }

  /*--------------------------------------.
  | internal::data_< cast_image_<T,I> >.  |
  `--------------------------------------*/

  namespace internal
  {
    template <typename I, typename N>
    inline
    data_< neighb::image<I, N> >::data_(I& ima, const N& nbh)
      : ima_(ima),
	nbh_(nbh)
    {
    }
  }

  /*----------------------.
  | neighb::image<I, N>.  |
  `----------------------*/

  namespace neighb
  {

    template <typename I, typename N>
    inline
    image<I, N>::image(Image<I>& ima, const Neighborhood<N>& nbh)
    {
      this->data_ =
	new mln::internal::data_< mln::neighb::image<I, N> >(exact(ima),
							     exact(nbh));
    }

    template <typename I, typename N>
    inline
    bool
    image<I, N>::has_data() const
    {
      return this->delegatee_()->has_data();
    }

    template <typename I, typename N>
    inline
    const N&
    image<I, N>::neighborhood() const
    {
      return this->data_->nbh_;
    }

  } // end of namespace mln::neighb

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#include <mln/neighb/get.hh>

#endif // ! MLN_NEIGHB_IMAGE_HH
