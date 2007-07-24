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

/*! \file mln/core/concept/doc/fast_image.hh
 * \brief This file documents the concept of mln::Fast_Image.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Fast_Image.
     * \see mln::Fast_Image
     */
    template <typename E>
    struct Fast_Image : public Image<E>
    {

      /*! \brief Give the border thickness.
       *
       * \pre The image has to be initialized.
       */
      unsigned border();

      /*! \brief Give the offset corresponding to the delta-point \p
       *  dp.
       *
       * \param[in] dp A delta-point.
       *
       * \pre The image has to be initialized.
       */
      int offset(const dpoint& dp);


      /*! \brief Give the point at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre The image has to be initialized.
       * \pre o < ncells()
       */
      point point_at_offset(unsigned o) const;


      /*! \brief Give a hook to the value buffer.
       *
       * \internal
       *
       * \pre The image has to be initialized.
       */
      const value* buffer() const;

      /*! \brief Read-only access to the image value at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre o < ncells()
       *
       * \return The value at \p o (not assignable).
       */
      rvalue operator[](unsigned o) const;


      /*! \brief Read-write access to the image value at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre o < ncells()
       *
       * \return The value at \p o (assignable).
       */
      lvalue operator[](unsigned o);

    };

  } // end of namespace mln::doc

} // end of namespace mln
