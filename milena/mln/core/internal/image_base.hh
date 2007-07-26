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

#ifndef MLN_CORE_INTERNAL_IMAGE_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_BASE_HH

/*! \file mln/core/internal/image_base.hh
 *
 * \brief Definition of a base class for some images.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace internal
  {


    template <typename I>
    struct morpher_lvalue_
    {
      typedef mln_lvalue(I) ret;
    };
    
    template <typename I>
    struct morpher_lvalue_< const I >
    {
      typedef mln_rvalue(I) ret;
    };



    template <typename Is_fast, typename E>
    struct select_image_concept_;

    template <typename E>
    struct select_image_concept_< metal::true_, E >
      : public Fast_Image<E>
    {};

    template <typename E>
    struct select_image_concept_< metal::false_, E >
      : public Image<E>
    {};



    /*! \brief A base class for images.
     *
     * \internal
     */
    template <typename S, typename E>
    struct image_base_ : public select_image_concept_< typename trait::is_fast<E>::ret,
						       E >
    {
      /// Point_Set associated type.
      typedef S pset;

      /// Point_Site associated type.
      typedef mln_psite(S) psite;

      /// Point associated type.
      typedef mln_point(S) point;


      /// Dpoint associated type.
      typedef mln_dpoint(point) dpoint;

      /// Coordinate associated type.
      typedef mln_coord(point) coord;


      /// Forward Point_Iterator associated type.
      typedef mln_fwd_piter(S) fwd_piter;

      /// Backward Point_Iterator associated type.
      typedef mln_bkd_piter(S) bkd_piter;


      /// Test if \p p belongs to the image domain.
      bool has(const psite& p) const;

      /// Give a bounding box of the image domain.
      const box_<point>& bbox() const;

      /// Give the number of points of the image domain.
      std::size_t npoints() const;

    protected:
      image_base_();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    bool
    image_base_<S,E>::has(const psite& p) const
    {
      mln_precondition(exact(this)->has_data());
      return exact(this)->domain().has(p);
    }

    template <typename S, typename E>
    const box_<mln_point(S)>&
    image_base_<S,E>::bbox() const
    {
      mln_precondition(exact(this)->has_data());
      return exact(this)->domain().bbox();
    }

    template <typename S, typename E>
    std::size_t
    image_base_<S,E>::npoints() const
    {
      mln_precondition(exact(this)->has_data());
      return exact(this)->domain().npoints();      
    }

    template <typename S, typename E>
    image_base_<S,E>::image_base_()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_BASE_HH
