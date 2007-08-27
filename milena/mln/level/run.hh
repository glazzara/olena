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

#ifndef MLN_LEVEL_RUN_HH
# define MLN_LEVEL_RUN_HH

/*! \file mln/level/run.hh
 *
 * \brief Run an accumulator onto image pixel values.
 */

# include <mln/level/take.hh>


namespace mln
{

  namespace level
  {

    /*! Run an accumulator onto the pixel values of the image \p input.
     *
     * \param[in] input The input image.
     * \param[in] a The accumulator.
     * \return A resulting accumulator.
     *
     * This routine runs: \n
     *   res = \p a \n
     *   res.init() \n
     *   level::take(res, \p input) \n
     *   return res \n
     */
    template <typename I, typename A>
    A run(const Image<I>& input, const Accumulator<A>& a);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename A>
    A run(const Image<I>& input, const Accumulator<A>& a)
    {
      A res = exact(a);
      res.init();
      level::take(res, input);
      return res;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_RUN_HH
