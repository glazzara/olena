// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

    /*! \brief Documentation class for mln::Site_Set.
     *
     * \see mln::Site_Set
     */
    template <typename E>
    struct Site_Set : public Object<E>
    {
      /*! \brief Site associated type.
       */
      typedef void site;

      /*! \brief PSite associated type.
       */
      typedef void psite;

      /*! \brief Forward Site_Iterator associated type.
       */
      typedef void fwd_piter;

      /*! \brief Backward Site_Iterator associated type.
       */
      typedef void bkd_piter;

      /*! \brief Test if \p p belongs to this site set. 
       *
       * \param[in] p A psite.
       * \return True if \p p is an element of the site set.
       */
      bool has(const psite& p) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
