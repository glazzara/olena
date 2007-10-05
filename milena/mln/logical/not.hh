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

#ifndef MLN_LOGICAL_NOT_HH
# define MLN_LOGICAL_NOT_HH

/*! \file mln/logical/not.hh
 *
 * \brief Point-wise "logical not" of a binary image.
 *
 * \todo Add static assertion and save one iterator in in-place version.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace logical
  {

    /*! Point-wise "logical not" of image \p input.
     *
     * \param[in] input the input image.
     * \param[out] output The result image.
     *
     * \pre \p output.domain == \p input.domain
     */
    template <typename I, typename O>
    void not_(const Image<I>& input, Image<O>& output);


    /*! Point-wise in-place "logical not" of image \p input.
     *
     * \param[in,out] input The target image.
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     input(p) = not input(p)
     */
    template <typename I>
    void not_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename O>
      void not__(trait::image::speed::any, const I& input,
		 trait::image::speed::any, O& output)
      {
	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = ! input(p);
      }

      template <typename I, typename O>
      void not__(trait::image::speed::fastest, const I& input,
		 trait::image::speed::fastest, O& output)
      {
	mln_pixter(const I) ip(input);
	mln_pixter(O)       op(output);
	for_all_2(ip, op)
	  op.val() = ! ip.val();
      }

    } // end of namespace mln::logical::impl


    // Facades.

    template <typename I, typename O>
    void not_(const Image<I>& input, Image<O>& output)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      impl::not__(mln_trait_image_speed(I)(), exact(input),
		  mln_trait_image_speed(O)(), exact(output));
    }

    template <typename I>
    void not_inplace(Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      impl::not__(mln_trait_image_speed(I)(), exact(input),
		  mln_trait_image_speed(I)(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::logical

} // end of namespace mln


#endif // ! MLN_LOGICAL_NOT_HH
