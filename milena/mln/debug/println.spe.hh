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

#ifndef MLN_DEBUG_PRINTLN_SPE_HH
# define MLN_DEBUG_PRINTLN_SPE_HH

/*! \file mln/debug/println.spe.hh
 *
 * \brief  Specializations for mln::debug::println.
 *
 * \todo Clean-up code.
 */

# ifndef MLN_DEBUG_PRINTLN_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DEBUG_PRINTLN_HH

# include <sstream>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/debug/format.hh>
# include <mln/debug/put_word.hh>
# include <mln/level/fill.hh>
# include <mln/accu/max.hh>


namespace mln
{

  namespace debug
  {

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

# ifdef MLN_CORE_SITE_SET_BOX2D_HH

      // 2D versions.

      inline
      void
      println(const box2d& b, const image2d<char>& input)
      {
	for (int row = b.min_row(); row <= b.max_row(); ++row)
	{
	  for (int col = b.min_col(); col <= b.max_col(); ++col)
	    std::cout << input.at(row, col) << ' ';
	  std::cout << std::endl;
	}
	std::cout << std::endl;
      }


      template <typename I>
      inline
      void
      println(const box2d& b, const I& input)
      {
	accu::max<unsigned> len_;
	mln_piter(I) p(input.domain());
	for_all(p)
	  {
	    std::ostringstream o;
	    o << format(input(p));
	    len_.take(o.str().length());
	  }
	unsigned len = len_ + 1;

	image2d<char> output(b.nrows(), b.ncols() * len, 0);
	level::fill(output, ' ');
	for_all(p)
	  {
	    std::ostringstream oss;
	    oss << format(input(p));
	    point2d w( p.row() - b.min_row(),
		      (p.col() - b.min_col()) * len);
	    put_word(output, w, oss.str());
	  }

	for (unsigned row = 0; row < b.nrows(); ++row)
	{
	  for (unsigned col = 0; col < b.ncols() * len; ++col)
	    std::cout << output.at(row, col);
	  std::cout << std::endl;
	}
	std::cout << std::endl;
      }

# endif // MLN_CORE_SITE_SET_BOX2D_HH

# ifdef MLN_CORE_IMAGE_IMAGE2D_H_HH

      // Hexa version.
      template <typename I>
      void
      println(const box2d_h& b, const hexa<I>& input)
      {
	typename hexa<I>::fwd_piter p(input.domain());

	int c = 1;
	int r = 1;
	int row_len = 1 + (b.max_col() - b.min_col()) / 2;

	for_all(p)
	  {
	    if (input.domain().has(p))
	      std::cout << format(input(p)) << "     ";
	    else
	      std::cout << "      ";

	    if (c >= row_len)
	    {
	      std::cout << std::endl;
	      if (r % 2)
		std::cout << "   ";
	      c = 0;
	      r++;
	    }
	    c++;
	  }
	std::cout << std::endl;
      }

# endif // MLN_CORE_IMAGE_IMAGE2D_H_HH


# ifdef MLN_CORE_SITE_SET_BOX3D_HH

      // 3D version.
      template <typename I>
      void
      println(const box3d& b, const I& input)
      {
	point3d p;
	def::coord& sli = p.sli();
	def::coord& row = p.row();
	def::coord& col = p.col();
	const int
	  max_row = b.max_row(),
	  max_sli = b.max_sli(),
	  max_col = b.max_col();

	for (sli = b.min_sli(); sli <= max_sli; ++sli)
	{
	  for (row = b.min_row(); row <= max_row; ++row)
	  {
	    for (int i = max_row; i >= row; --i)
	      std::cout << ' ';
	    for (col = b.min_col(); col <= max_col; ++col)
	      if (input.domain().has(p))
		std::cout << format(input(p)) << ' ';
	      else
		std::cout << "  ";
	    std::cout << std::endl;
	  }
	  std::cout << std::endl;
	}
      }

# endif // MLN_CORE_SITE_SET_BOX3D_HH

    } // end of namespace mln::debug::impl

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_SPE_HH
