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

/*! \file mln/core/concept/doc/image.hh
 * \brief This file documents the concept of mln::Image.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Image.
     * \see mln::Image
     */
    template <typename E>
    struct Image : public Object<E>
    {

      /*! \brief Value associated type.
       *
       *  \invariant This type is neither qualified by const, nor by
       * reference.
       */
      typedef void value;

      /*! \brief Type returned by the read pixel value operator.
       */
      typedef void rvalue;

      /*! \brief Type returned by the read-write pixel value operator.
       */
      typedef void lvalue;

      /*! \brief Value set associated type.
       * \invariant This type has to derive from mln::Value_Set.
       */
      typedef void vset;



      /*! \brief Associate type that describes how this type of image
       * is constructed.
       */
      typedef void skeleton;



      /*! \brief Test if the image have been initialized.
       */
      bool is_valid() const;

      /*! \brief Test if the image owns the point site \p p.
       *
       * \return True if accessing the image value at \p p is
       * possible, that is, does not abort the execution.
       */
      bool has(const psite& p) const;

      /*! \brief Give the definition domain of the image.
       *
       * \return A reference to the domain point set.
       */
      const pset& domain() const;

      /*! \brief Give the set of values of the image.
       *
       * \return A reference to the value set.
       */
      const vset& values() const;

      /*! \brief Read-only access to the image value located at \p p.
       *
       * \param[in] p A point site.
       *
       * \pre The image has to own the site \p p.
       *
       * \return The value at \p p (not assignable).
       */
      rvalue operator()(const psite& p) const;

      /*! \brief Read-write access to the image value located at \p p.
       *
       * \param[in] p A point site.
       *
       * \pre The image has to own the site \p p.
       *
       * \return The value at \p p (assignable).
       */
      lvalue operator()(const psite& p);



      /*! \brief Point set associated type.
       * \invariant This type has to derive from mln::Site_Set.
       */
      typedef void pset;

      /*! \brief Site associated type.
       * \invariant This type has to derive from mln::Point.
       */
      typedef void point;

      /*! \brief Point_Site associated type.
       * \invariant This type has to derive from mln::Point_Site.
       */
      typedef void psite;


      /*! \brief Coordinate associated type.
       */
      typedef void coord;

      /*! \brief Dpsite associated type.
       * \invariant This type has to derive from mln::Dpoint.
       */
      typedef void dpoint;


      /*! \brief Forward point iterator associated type.
       * \invariant This type has to derive from mln::Site_Iterator.
       */
      typedef void fwd_piter;

      /*! \brief Backward point iterator associated type.
       * \invariant This type has to derive from mln::Site_Iterator.
       */
      typedef void bkd_piter;


      /*! \brief Test if \p p belongs to the image domain. 
       *
       * \param[in] p A point site.
       *
       * \return True if \p p belongs to the image domain.
       *
       * \invariant has(p) is true => has(p) is also true.
       */
      bool has(const psite& p) const;

      /*! \brief Give a bounding box of the image domain. 
       *
       * This bounding box may be larger than the smallest bounding
       * box (the optimal one).  Practically an image type is not
       * obliged to update its bounding box so that it is always
       * optimal.
       *
       * \return A bounding box of the image domain.
       */
      const box<point>& bbox() const;

      /*! \brief Give the number of points of the image domain. 
       */
      unsigned nsites() const;
    };


  } // end of namespace mln::doc

} // end of namespace mln
