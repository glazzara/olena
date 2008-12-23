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

#ifndef MLN_CORE_ROUTINE_CLONE_HH
# define MLN_CORE_ROUTINE_CLONE_HH

/*! \file mln/core/routine/duplicate.hh
 *
 * \brief Clone an image, that is, get an effective copy.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/routine/init.hh>
# include <mln/data/fill.hh>


namespace mln
{

  /*! \brief Clone the image \p model with the values of the image \p
   *  data.
   *
   * \param[in] model The image to be duplicateed.
   * \result The duplicate.
   *
   * \pre model.has_data
   */
  template <typename I>
  mln_concrete(I) duplicate(const Image<I>& model);


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  inline
  mln_concrete(I) duplicate(const Image<I>& model)
  {
    // FIXME: Add a static check that mln_concrete(I) actually *is* concrete...
    trace::entering("core::duplicate");

    mln_concrete(I) tmp;
    initialize(tmp, model);
    data::fill(tmp, model);

    trace::exiting("core::duplicate");
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_ROUTINE_CLONE_HH
