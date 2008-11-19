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

#ifndef MLN_GEOM_RESIZE_HH
# define MLN_GEOM_RESIZE_HH

/*! \file mln/geom/resize.hh
 *
 *  \brief Resize an image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/routine/clone.hh>
# include <mln/make/w_window1d.hh>
# include <mln/accu/mean.hh>
# include <mln/border/resize.hh>
# include <mln/border/fill.hh>
# include <mln/debug/println.hh>


namespace mln
{

  namespace geom
  {

    /*!
     * \brief Resize an image \p input_ with \p ratio.
     *
     * \param[in] input_ The image to resize.
     * \param[in] ratio The ratio of the resize image.
     *
     * \pre \p input_ has to be initialized.
     * \pre \p ratio <= 1.
     *
     * \return The resized image.
     */
    template <typename I>
    I
    resize(const Image<I>& input_, const float ratio);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I>
      I
      resize_1d_(const I& input, const float ratio)
      {
	trace::entering("mln::geom::impl::resize_1d_");
	typedef mln_value(I) V;

	unsigned cols = input.bbox().len(0);
	unsigned new_cols = (unsigned)(ratio * cols);

	unsigned n = (unsigned)(1 / ratio);

	I output (new_cols);
	level::fill(output, 0);

	for (unsigned j = 0; j < cols; ++j)
	  {
	    output(point1d(j / n)) += input(point1d(j));
	    if (!((j + 1) % n))
	      output(point1d(j / n)) /= n;
	  }

	trace::exiting("mln::geom::impl::resize_1d_");
	return output;
      }

      template <typename I>
      I
      resize_2d_(const I& input, const float ratio)
      {
	trace::entering("mln::geom::impl::resize_2d_");
	typedef mln_sum(mln_value(I)) V;

	unsigned rows = input.bbox().len(0);
	unsigned cols = input.bbox().len(1);
	unsigned new_rows = (unsigned)(ratio * rows);
	unsigned new_cols = (unsigned)(ratio * cols);

	unsigned n = (unsigned)(1 / ratio);
	unsigned nn = n * n;

	I output (new_rows, new_cols);
	std::vector<V> v (new_cols);

	for (unsigned i = 0; i < rows; ++i)
	  {
	    unsigned in = i / n;

	    if (!(i % n))
	      for (unsigned j = 0; j < new_cols; ++j)
		v[j] = 0;

	    for (unsigned j = 0; j < cols; ++j)
	      v[j / n] += input(point2d(i, j));

	    if (!((i + 1) % n))
	      for (unsigned j = 0; j < cols; ++j)
		output(point2d(in, j / n)) = v[j / n] / nn;
	  }

	trace::exiting("mln::geom::impl::resize_2d_");
	return output;
      }

    } // end of namespace mln::geom::impl


    template <typename I>
    I
    resize(const Image<I>& input_, const float ratio)
    {
      trace::entering("mln::geom::resize");

      const I input = exact (input_);
      mln_precondition(input.has_data());
      mln_precondition(ratio <= 1.0f);
      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      I output;
      typedef mln_psite(I) P;

      /// FIXME : Find a better way for the disjunction.

//       if (P::dim == 1)
// 	output = impl::resize_1d_(input, ratio);

      if (P::dim == 2)
	output = impl::resize_2d_(input, ratio);

      trace::exiting("mln::geom::resize");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_RESIZE_HH
