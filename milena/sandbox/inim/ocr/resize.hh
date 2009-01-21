// Copyright (C) 2008 EPITA Research and Development Laboratory
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

# include <mln/win/rectangle2d.hh>

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
     * \pre \p ratio >= 1.
     *
     * \return The resized image.
     */
    template <typename I>
    mln_concrete(I)
    resize(const Image<I>& input_, const unsigned ratio);

# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    mln_concrete(I)
    resize(const Image<I>& input_, const unsigned ratio)
    {
      trace::entering("mln::geom::resize");

      typedef mln_concrete(I) O;

      const I input = exact (input_);

      std::size_t rows = input.domain().len(0);
      std::size_t cols = input.domain().len(1);
      std::size_t new_rows = ratio * rows;
      std::size_t new_cols = ratio * cols;

      O output(new_rows, new_cols);

      mln_piter(O) p(output.domain());

      for_all(p)
      {
	output(p) = input(point2d(p[0] / ratio, p[1] / ratio)) ? 255 : 0;
      }

      trace::exiting("mln::geom::resize");
      return output;

    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::geom

} // end of namespace mln



#endif // ! MLN_GEOM_RESIZE_HH
