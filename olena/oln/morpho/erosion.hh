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

#ifndef	OLN_MORPHO_EROSION_HH
# define OLN_MORPHO_EROSION_HH

# include <oln/level/apply_local.hh>
# include <oln/level/compare.hh>
# include <oln/border/fill.hh>
# include <oln/accumulator/min.hh>
# include <oln/accumulator/and.hh>

// FIXME: Add guard.
# include <oln/core/2d/rectangle2d.hh>
# include <oln/core/2d/hline2d.hh>
# include <oln/core/2d/vline2d.hh>


namespace oln
{

  namespace morpho
  {

    // Fwd decl.

    template <typename I, typename W>
    oln_plain(I)
    erosion(const Image<I>& input, const Window<W>& win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic versions.

      template <typename I, typename W>
      oln_plain(I)
      erosion_on_function_(const Image<I>& input, const Window<W>& win)
      {
	border::fill(input, oln_max(oln_value(I)));
	accumulator::min_<oln_value(I)> min;
	return level::apply_local(min, input, win);
      }

      template <typename I, typename W>
      oln_plain(I)
      erosion_on_set_(const Image<I>& input, const Window<W>& win)
      {
	border::fill(input, true);
	accumulator::and_<oln_value(I)> accu_and;
	return level::apply_local(accu_and, input, win);
      }


      // FIXME: Add guard here.
      // Versions for rectangles.

      template <typename I>
      oln_plain(I)
      erosion_on_function_(const Image_2D<I>& input, const rectangle2d& win)
      {
	hline2d hl(win.half_width());
	vline2d hv(win.half_height());
	oln_plain(I) tmp = erosion(input, hl);
	return erosion(tmp, hv);
      }

      // FIXME: Add other fast versions.


      // Impl facade.

      template <typename I, typename W>
      oln_plain(I) erosion_(const Image<I>& input, const Window<W>& win)
      {
	return erosion_on_function_(exact(input), exact(win));
      }

      template <typename I, typename W>
      oln_plain(I) erosion_(const Binary_Image<I>& input, const Window<W>& win)
      {
	return erosion_on_set_(exact(input), exact(win));
      }

    } // end of namespace oln::morpho::impl


    // Facade.

    template <typename I, typename W>
    oln_plain(I)
    erosion(const Image<I>& input, const Window<W>& win)
    {
      oln_plain(I) output = impl::erosion_(exact(input), exact(win));
      if (win.is_centered())
	postcondition(output <= input);
      return output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_EROSION_HH
