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

#ifndef	OLN_LINEAR_CONVOLUTION_HH
# define OLN_LINEAR_CONVOLUTION_HH

# include <oln/core/concept/image.hh>
# include <oln/core/concept/weighted_window.hh>
# include <oln/core/internal/f_weighted_window.hh>
# include <oln/core/internal/f_ch_value.hh>
# include <oln/convert/to_dpoint.hh>


namespace oln
{

  namespace linear
  {

    // Fwd decls.

    template <typename V, typename I, typename J>
    oln_plain_value(I, V)
    convolution(const Image<I>& f, const Image<J>& g);

    template <typename V, typename I, typename W, unsigned n>
    oln_plain_value(I, V)
    convolution(const Image<I>& f, const W (&values)[n]);

    template <typename V, typename I, typename W>
    oln_plain_value(I, V)
    convolution(const Image<I>& f, const Weighted_Window<W>& w_win);


# ifndef OLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic versions.

      template <typename V, typename I, typename J>
      oln_plain_value(I, V)
      convolution_image_(const Point_Wise_Accessible_Image<I>& f, const Image<J>& g)
      {
	oln_plain_value(I, V) output;
	prepare(output, with, f);
	oln_piter(I) p(f.points());
	oln_piter(J) q(g.points());
	for_all(p)
	  {
	    V val = 0;
	    for_all(q)
	      {
		oln_point(I) p_q = p - convert::to_dpoint(q);
		if (f.has(p_q))
		  val += g(q) * f(p_q);
	      }
	    output(p) = val;
	  }
	return output;
      }

      template <typename V, typename I, typename W>
      oln_plain_value(I, V)
      convolution_window_(const Point_Wise_Accessible_Image<I>& input, const W& w_win)
      {
	oln_plain_value(I, V) output;
	prepare(output, with, input);
	oln_piter(I) p(input.points());
	for_all(p)
	  {
	    V val = 0;
	    for (unsigned i = 0; i < w_win.size(); ++i)
	      {
		oln_point(I) q = p + w_win.dp(i);
		if (input.has(q))
		  val += w_win.w(i) * input(q);
	      }
	    output(p) = val;
	  }
	return output;
      }

    } // end of namespace oln::linear::impl


    // Facades.

    template <typename V, typename I, typename J>
    oln_plain_value(I, V)
    convolution(const Image<I>& f, const Image<J>& g)
    {
      mlc::assert_< mlc_is_a(I, Point_Wise_Accessible_Image) >::check();
      oln_plain_value(I, V) output = impl::convolution_image_<V>(exact(f), exact(g));
      return output;
    }

    template <typename V, typename I, typename W>
    oln_plain_value(I, V)
    convolution(const Image<I>& input, const Weighted_Window<W>& w_win)
    {
      mlc::assert_< mlc_is_a(I, Point_Wise_Accessible_Image) >::check();
      oln_plain_value(I, V) output = impl::convolution_window_<V>(exact(input), exact(w_win));
      return output;
    }

    template <typename V, typename I, typename W, unsigned n>
    oln_plain_value(I, V)
    convolution(const Image<I>& input, const W (&values)[n])
    {
      mlc::assert_< mlc_is_a(I, Point_Wise_Accessible_Image) >::check();
      oln_f_weighted_window(W, oln_dpoint(I)) w_win;
      w_win.fill_with(values);
      return linear::convolution<V>(input, w_win);
    }

# endif // ! OLN_INCLUDE_ONLY

  } // end of namespace oln::linear

} // end of namespace oln


#endif // ! OLN_LINEAR_CONVOLUTION_HH
