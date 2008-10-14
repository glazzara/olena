// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

# include <mln/level/update.hh>
# include <mln/core/concept/meta_accumulator.hh>



namespace mln
{

  namespace level
  {

    /*! Compute an accumulator onto the pixel values of the image \p input.
     *
     * \param[in] a An accumulator.
     * \param[in] input The input image.
     * \return The accumulator result.
     *
     * It fully relies on level::update.
     */
    template <typename A, typename I>
    mln_result(A)
    compute(const Accumulator<A>& a, const Image<I>& input);


    /*! Compute an accumulator onto the pixel values of the image \p input.
     *
     * \param[in] a A meta-accumulator.
     * \param[in] input The input image.
     * \return The accumulator result.
     */
    template <typename A, typename I>
    mln_accu_with(A, mln_value(I))::result
    compute(const Meta_Accumulator<A>& a, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    // Facades.

    template <typename A, typename I>
    inline
    mln_result(A)
    compute(const Accumulator<A>& a_, const Image<I>& input)
    {
      trace::entering("level::compute");

      A a = exact(a_);
      level::internal::update_tests(a, input);

      a.init();
      level::internal::update_dispatch(a, input);

      trace::exiting("level::compute");
      return a;
    }

    template <typename A, typename I>
    inline
    mln_accu_with(A, mln_value(I))::result
    compute(const Meta_Accumulator<A>&, const Image<I>& input)
    {
      mln_accu_with(A, mln_value(I)) accu;
      return compute(accu, input); // Call the previous version.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_COMPUTE_HH
