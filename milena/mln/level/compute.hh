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

#ifndef MLN_LEVEL_COMPUTE_HH
# define MLN_LEVEL_COMPUTE_HH

/*! \file mln/level/compute.hh
 *
 * \brief Compute an accumulator onto image pixel values.
 */

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/level/take.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  namespace level
  {

    /*! Compute an accumulator onto the pixel values of the image \p input.
     *
     * \param[in] input The input image.
     * \param[in] a The accumulator.
     * \return The accumulator result.
     *
     * This routine runs: \n
     *   tmp = \p a \n
     *   tmp.init() \n
     *   level::take(\p input, tmp) \n
     *   return tmp.to_result() \n
     */
    template <typename I, typename A>
    mln_result(A)
    compute(const Image<I>& input, const Accumulator<A>& a);

    template <typename A, typename I>
    mln_accu_with(A, mln_value(I))::result
    compute(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename A>
    inline
    mln_result(A)
    compute(const Image<I>& input, const Accumulator<A>& a_)
    {
      trace::entering("level::compute");

      mln_precondition(exact(input).has_data());
      A a = exact(a_); // Cpy.
      a.init();
      level::take(input, a);

      trace::exiting("level::compute");
      return a.to_result();
    }

    template <typename A, typename I>
    inline
    mln_result(A)
    compute(const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      return level::compute(input, A());
    }

    template <typename A, typename I>
    inline
    mln_accu_with(A, mln_value(I))::result
    compute(const Image<I>& input)
    {
      mlc_is_a(A, Meta_Accumulator)::check();
      return compute(input,
		     mln_accu_with(A, mln_value(I))());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_COMPUTE_HH
