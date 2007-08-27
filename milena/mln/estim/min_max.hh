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

#ifndef MLN_ESTIM_MIN_MAX_HH
# define MLN_ESTIM_MIN_MAX_HH

/*! \file mln/estim/min_max.hh
 *
 * \brief Compute the min and max pixel values of an image.
 */

# include <mln/accu/min_max.hh>
# include <mln/level/take.hh>


namespace mln
{

  namespace estim
  {

    /*! \brief Compute the min and max values of the pixels of image \p input.
     *
     * \param[in] input The image.
     * \param[out] min The minimum pixel value of \p input.
     * \param[out] max The maximum pixel value of \p input.
     */
    template <typename I>
    void min_max(const Image<I>& input,
		 mln_value(I)& min, mln_value(I)& max);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void min_max(const Image<I>& input,
		 mln_value(I)& min, mln_value(I)& max)
    {
      mln_precondition(exact(input).has_data());
      accu::min_max<mln_value(I)> a;
      a.init();
      level::take(a, input);
      min = a.min();
      max = a.max();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::estim

} // end of namespace mln


#endif // ! MLN_ESTIM_MIN_MAX_HH
