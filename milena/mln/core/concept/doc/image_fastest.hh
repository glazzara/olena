// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for the concept of images that have
     * the speed property set to "fastest".
     */
    template <typename E>
    struct Fastest_Image : public Image<E>
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
      int delta_offset(const dpoint& dp);


      /*! \brief Give the point at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre The image has to be initialized.
       * \pre o < nelements()
       */
      point point_at_offset(unsigned o) const;


      /*! \brief Give a hook to the value buffer.
       *
       *
       *
       * \pre The image has to be initialized.
       */
      const value* buffer() const;

      /*! \brief Read-only access to the image value at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre o < nelements()
       *
       * \return The value at \p o (not assignable).
       */
      rvalue operator[](unsigned o) const;


      /*! \brief Read-write access to the image value at offset \p o.
       *
       * \param[in] o An offset.
       *
       * \pre o < nelements()
       *
       * \return The value at \p o (assignable).
       */
      lvalue operator[](unsigned o);


      /*! \brief Give the number of pixels of the image including
       *  those of the virtual border.
       *
       * \pre The image has to be initialized.
       */
      unsigned nelements() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
