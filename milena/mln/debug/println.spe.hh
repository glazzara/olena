// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DEBUG_PRINTLN_SPE_HH
# define MLN_DEBUG_PRINTLN_SPE_HH

/// \file
///
/// Specializations for mln::debug::println.
///
/// \todo Clean-up code.

# ifndef MLN_DEBUG_PRINTLN_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DEBUG_PRINTLN_HH

# include <sstream>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/debug/format.hh>
# include <mln/debug/put_word.hh>
# include <mln/data/fill.hh>
# include <mln/accu/stat/max.hh>
# include <mln/opt/at.hh>

// FIXME: do not include all these headers
# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/box2d_h.hh>
# include <mln/core/alias/box3d.hh>

namespace mln
{
  // Forward declaration.
  template <typename I> struct hexa;

  namespace debug
  {

    namespace impl
    {

      void println(const box2d& b, const image2d<char>& input);

      template <typename I>
      void println(const box2d& b, const I& input);

      template <typename I>
      void println(const box2d_h& b, const hexa<I>& input);

      template <typename I>
      void println(const box3d& b, const I& input);


# ifndef MLN_INCLUDE_ONLY

      // 2D versions.

      inline
      void
      println(const box2d& b, const image2d<char>& input)
      {
	for (def::coord row = b.min_row(); row <= b.max_row(); ++row)
	  {
	    for (def::coord col = b.min_col(); col <= b.max_col(); ++col)
	      std::cout << opt::at(input, row, col) << ' ';
	    std::cout << std::endl;
	  }
	std::cout << std::endl;
      }


      template <typename I>
      inline
      void
      println(const box2d& b, const I& input)
      {
	accu::stat::max<unsigned> len_;
	mln_piter(I) p(input.domain());
	for_all(p)
	{
	  std::ostringstream o;
	  o << format(input(p));
	  len_.take(o.str().length());
	}
	unsigned len = len_ + 1;

	image2d<char> output(b.nrows(), b.ncols() * len, 0);
	data::fill(output, ' ');
	for_all(p)
	{
	  std::ostringstream oss;
	  oss << format(input(p));
	  def::coord
	    row = static_cast<def::coord>(p.row() - b.min_row()),
	    col = static_cast<def::coord>((p.col() - b.min_col()) * len);
	  point2d w(row, col);
	  put_word(output, w, oss.str());
	}

	for (def::coord row = 0; row < def::coord(b.nrows()); ++row)
	  {
	    for (def::coord col = 0; col < def::coord(b.ncols() * len); ++col)
	      std::cout << opt::at(output, row, col);
	    std::cout << std::endl;
	  }
	std::cout << std::endl;
      }

      // Hexa version.
      template <typename I>
      void
      println(const box2d_h& b, const hexa<I>& input)
      {
	mln_piter(hexa<I>) p(input.domain());

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

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::debug::impl

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_SPE_HH
