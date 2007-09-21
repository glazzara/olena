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

#ifndef MLN_CORE_CLONE_HH
# define MLN_CORE_CLONE_HH

/*! \file mln/core/clone.hh
 *
 * \brief Clone an image, that is, get an effective copy.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  /*! Clone the image \p ima with the values of the image \p data.
   *
   * \param[in,out] ima The image to be cloneed.
   * \param[in] data The image.
   *
   * \warning The definition domain of \p ima has to be included in
   * the one of \p data.
   *
   * \pre \p ima.domain <= \p data.domain.
   *
   * \todo Use memcpy when possible.
   */
  template <typename I>
  mln_concrete(I) clone(const Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY

  namespace impl
  {

    template <typename I>
    void clone_(mln_concrete(I)& result, const Image<I>& ima)
    {
      std::cerr << "oops" << std::endl; // FIXME: Fake code.
    }    

  } // end of namespace mln::impl


  template <typename I>
  mln_concrete(I) clone(const Image<I>& ima)
  {
    mln_concrete(I) tmp;
    impl::clone_(ima, tmp);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CLONE_HH
