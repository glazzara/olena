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

#ifndef MLN_ACCU_COMPUTE_HH
# define MLN_ACCU_COMPUTE_HH

/*! \file mln/accu/compute.hh
 *
 * \brief Define an accumulator that computes image pixels.
 */

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/metal/is_a.hh>
# include <mln/util/pix.hh>


namespace mln
{

  namespace accu
  {

    /*! Make an accumulator compute the pixels of the image \p input.
     *
     * \param[in] input The input image.
     *
     * This routine runs: \n
     *   a.take(make::pix(input, p));
     *   on all pixels on the images.
     *
     * \warning This routine does not perform a.init().
     */
    template <typename A, typename I>
    mln_result(A)
    compute(const Image<I>& input);


    // FIXME: Doc!
    template <typename A, typename I>
    mln_accu_with(A, util::pix<I>)::result
    compute(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    template <typename A, typename I>
    inline
    mln_result(A)
    compute(const Image<I>& input_)
    {
      mlc_is_a(A, Accumulator)::check();
      const I& input = exact(input_);
      A a;
      mln_piter(I) p(input.domain());
      for_all(p)
	a.take(make::pix(input, p));
      return a.to_result();
    }

    template <typename A, typename I>
    inline
    mln_accu_with(A, util::pix<I>)::result
    compute(const Image<I>& input)
    {
      mlc_is_a(A, Meta_Accumulator)::check();
      typedef mln_accu_with(A, util::pix<I>) A_;
      return compute<A_>(input);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_COMPUTE_HH
