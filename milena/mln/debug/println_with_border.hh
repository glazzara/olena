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

#ifndef MLN_DEBUG_PRINTLN_WITH_BORDER_HH
# define MLN_DEBUG_PRINTLN_WITH_BORDER_HH

/*! \file mln/debug/println_with_border.hh
 *
 * \brief Print an image on the standard output.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/debug/format.hh>

# include <mln/core/box2d.hh>
# include <mln/core/box3d.hh>

namespace mln
{

  namespace debug
  {

    /// Print the image \p input on the standard output.
    template <typename I>
    void println_with_border(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // generic version
      template <typename S, typename I>
      void println_with_border(const S&, const I& input)
      {
 	for (size_t i = 0; i < input.ncells(); i++)
	  std::cout << format( input.buffer()[i] ) << ' ';
	std::cout << std::endl;
      }

      // 2D version
      template <typename I>
      void println_with_border(const box2d& b, const I& input)
      {
	const std::size_t ncols = b.ncols() + 2 * input.border();
 	for (size_t i = 0; i < input.ncells(); i++)
	{
	  std::cout << format( input.buffer()[i] ) << ' ';
	  if (((i+1) % ncols) == 0)
	    std::cout << std::endl;
	}
	std::cout << std::endl;
      }

      // 3D version
      template <typename I>
      void println_with_border(const box3d& b, const I& input)
      {
	typedef mln_point(I) P;

	std::size_t len_s = b.len(P::dim - 3);
	std::size_t len_r = b.len(P::dim - 2);
	std::size_t len_c = b.len(P::dim - 1);

	std::size_t border = input.border ();
	std::size_t real_len_s = len_s + 2 * border;
	std::size_t real_len_c = len_c + 2 * border;
	std::size_t real_len_r = len_r + 2 * border;
    
	for (std::size_t k = 0; k < real_len_s; ++k)
	  {
	    for (std::size_t j = 0; j < real_len_r; ++j)
	      {
		for (std::size_t i = 0; i < real_len_c; ++i)
		  std::cout << format(input[k * (real_len_r * real_len_c) + j * real_len_c + i])
			    << ' ';
		std::cout << std::endl;
	      }
	    std::cout << std::endl;
	  }
	std::cout << std::endl;
      }
    } // end of namespace mln::debug::impl



    // facade
    template <typename I>
    void println_with_border(const Image<I>& input)
    {
      mlc_is(mln_trait_image_speed(I), trait::image::speed::fastest)::check();

      impl::println_with_border(exact(input).bbox(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_WITH_BORDER_HH
