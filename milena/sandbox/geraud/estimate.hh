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

#ifndef MLN_LABELING_ESTIMATE_HH
# define MLN_LABELING_ESTIMATE_HH

/*! \file mln/labeling/estimate.hh
 *
 * \brief Compute the estimate pixel value.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling.hh>

# include <mln/accu/compute.hh>
// # include <mln/core/image_if_value.hh>



namespace mln
{

  namespace labeling
  {

    /*! \brief Label a binary image and compute some estimations over
     *  the components.
     *
     * \param[in] input The image.
     *
     * The free parameter \c A is a type of accumulator.
     *
     * FIXME: Not yet impled.
     */
    template <typename A, typename I>
    void
    estimate(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename A, typename I>
    void
    estimate(const Image<I>& input)
    {
      trace::entering("labeling::estimate");
      mlc_equal(mln_trait_image_kind(I), mln::trait::image::kind::binary)::check();
      mln_precondition(exact(input).has_data());

      std::cerr << "Sorry: mln::labeling::estimate is not yet implemented!" << std::endl;

      // FIXME: Was:
      // mln_accu_with(A, util::pix<I>)::result res = accu::compute<A>(input | val);

      trace::exiting("labeling::estimate");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_ESTIMATE_HH
