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

#ifndef MLN_DEBUG_PRINTLN_HH
# define MLN_DEBUG_PRINTLN_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/box2d.hh>


namespace mln
{

  namespace debug
  {

    template <typename I>
    void println(const Image<I>& input);

    template <typename I, typename W>
    void println(const Image<I>& input_,
		 const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename S, typename I>
      void println(const S&, const Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_piter(I) p(input.domain());
	for_all(p)
	  std::cout << input(p) << ' ';
	std::cout << std::endl;
      }

      template <typename I>
      void println(const box2d& b,
		   const I& input)
      {
	for (int row = b.pmin().row(); row <= b.pmax().row(); ++row)
	  {
	    for (int col = b.pmin().col(); col <= b.pmax().col(); ++col)
	      std::cout << input(mk_point2d(row, col)) << ' ';
	    std::cout << std::endl;
	  }
      }

    } // end of namespace mln::debug::impl



    // facade

    template <typename I>
    void println(const Image<I>& input)
    {
      impl::println(exact(input).domain(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_HH
