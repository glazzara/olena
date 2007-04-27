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

#ifndef OLN_CONVERT_TO_WINDOW_HH
# define OLN_CONVERT_TO_WINDOW_HH

# include <oln/core/concept/image.hh>
# include <oln/core/internal/f_image_to_window.hh>
# include <oln/convert/to_dpoint.hh>


namespace oln
{

  namespace convert
  {

    // Fwd decls.

    namespace internal
    {
      template <unsigned n>
      struct to_window_values_;
    }

    template <typename I>
    oln_f_image_to_window(I)
    to_window(const Binary_Image<I>& input);

    template <unsigned n>
    internal::to_window_values_<n>
    to_window(const bool (&values)[n]);
    // FIXME: Wrong semantics: to_window should really give a window...


# ifndef OLN_INCLUDE_ONLY

    // Generic version.

    template <typename I>
    oln_f_image_to_window(I)
    to_window(const Binary_Image<I>& input)
    {
      oln_f_image_to_window(I) win;
      oln_dpoint(I) dp;
      oln_piter(I) p(input.points());
      for_all(p)
	if (input(p) == true)
	  win.take(convert::to_dpoint(p));
      return win;
    }


    namespace internal
    {

      template <typename G, typename W, unsigned n>
      void fill_(W& win, const bool (&values)[n])
      {
	int h = int(std::sqrt(n)) / 2;
	precondition((2 * h + 1) * (2 * h + 1) == n);
	unsigned i = 0;
	for (int drow = -h; drow <= h; ++drow) // FIXME: Replace 'int' by 'oln_coord(W)'
	  for (int dcol = -h; dcol <= h; ++dcol)
	    if (values[i++] == true)
	      win.take(oln_dpoint(W)(drow, dcol));
      }

      template <unsigned n>
      struct to_window_values_
      {
	const bool (&values_)[n];

	to_window_values_(const bool (&values)[n])
	  : values_(values)
	{}

	template <typename W>
	operator W() const
	{
	  mlc::assert_< mlc_is_a(W, Window) >::check(); // FIXME: Add err msg.
	  W tmp;
	  fill_<oln_grid(W)>(tmp, values_);
	  return tmp;
	}
      };

    } // end of namespace oln::convert::internal


    template <unsigned n>
    internal::to_window_values_<n>
    to_window(const bool (&values)[n])
    {
      return internal::to_window_values_<n>(values);
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::convert

} // end of namespace oln


#endif // ! OLN_CONVERT_TO_WINDOW_HH
