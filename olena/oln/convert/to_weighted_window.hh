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

#ifndef OLN_CONVERT_TO_WEIGHTED_WINDOW_HH
# define OLN_CONVERT_TO_WEIGHTED_WINDOW_HH

# include <oln/core/concept/image.hh>
# include <oln/core/concept/window.hh>
# include <oln/core/concept/function.hh>
# include <oln/core/internal/f_weighted_window.hh>


namespace oln
{

  namespace convert
  {

    // Fwd decl.

    template <typename I>
    oln_f_image_to_weighted_window(I)
    to_weighted_window(const Image<I>& input);


    template <typename F, typename W>
    oln_f_weighted_window(oln_result(F), oln_dpoint(W))
      to_weighted_window(const Function_p2v<F>& weight, // FIXME: should be Function_dp2v?
		       const Window<W>& win);


# ifndef OLN_INCLUDE_ONLY

    // Generic versions.

    template <typename I>
    oln_f_image_to_weighted_window(I)
    to_weighted_window(const Image<I>& input)
    {
      oln_point(I) O; O.set_all(0);
      oln_f_image_to_weighted_window(I) output;
      oln_piter(I) p(input.points());
      for_all(p)
	output.take(input(p), O - p.to_point()); // FIXME: zero - p
      return output;
    }

    template <typename F, typename W>
    oln_f_weighted_window(oln_result(F), oln_dpoint(W))
    to_weighted_window(const Function_p2v<F>& weight, const Window<W>& win)
    {
      const F& weight_ = exact(weight);
      oln_f_weighted_window(oln_result(F), oln_dpoint(W)) output;
      oln_point(W) O; O.set_all(0);
      oln_qiter(W) q(win, O);
      for_all(q)
	output.take(weight_(q), O - q.to_point());
      return output;
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::convert

} // end of namespace oln


#endif // ! OLN_CONVERT_TO_WEIGHTED_WINDOW_HH
