// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_DOC_GENERALIZED_PIXEL_HH
# define MLN_CORE_CONCEPT_DOC_GENERALIZED_PIXEL_HH

/*! \file
 * \brief This file documents the concept of mln::Generalized_Pixel.
 */


namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Generalized_Pixel.
     *
     * \see mln::Generalized_Pixel
     */
    template <typename E>
    struct Generalized_Pixel
    {

      /// Image associated type (with possible const qualification).
      typedef void image;

      /// Value associated type.
      typedef void value;

      /// Read-only value associated type.
      typedef void rvalue;


      /*! \brief Give the value of this generalized pixel.
       *
       * \return A read-only value.
       */
      rvalue val() const;

      /*! \brief Give the image of this generalized pixel.
       *
       * The constness of a pixel object is not transmitted to the
       * underlying image.
       */
      image& ima() const;

      /*! \brief Give a hook to the value address.
       *
       * This method allows for iterators to refer to a generalized
       * pixel.
       *
       * \return A pointer to the value address.
       */
      mln_qlf_value(ima)** address_() const;

    protected:
      Generalized_Pixel();
    };

  } // end of namespace mln::doc

} // end of namespace mln
