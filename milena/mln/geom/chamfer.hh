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

#ifndef MLN_GEOM_CHAMFER_HH
# define MLN_GEOM_CHAMFER_HH

/*! \file mln/geom/chamfer.hh
 *
 * \brief Connected component chamfer of the image objects.
 */

# include <mln/level/fill.hh>
# include <mln/core/alias/w_window2d_int.hh>
# include <mln/core/alias/w_window2d_float.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image_if_value.hh>
# include <mln/core/image/inplace.hh>
# include <mln/canvas/chamfer.hh>

namespace mln
{

  namespace geom
  {

    /// Apply chamfer algorithm to a binary image.
    template <typename I, typename W>
    mln_ch_value( I, unsigned )
      chamfer(const Image<I>& input_, const W& w_win_,
	      unsigned max = mln_max(unsigned));


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // \internal Functors.

      template <typename I_, typename W_>
      struct chamfer_t
      {
	typedef I_ I;
	typedef W_ W;
	typedef mln_point(I_) P;

	// requirements from mln::canvas::chamfer:

	const I& input;
	const W& win;

	mln_ch_value(I_, unsigned) output;
	bool status;
	unsigned max;

 	inline
 	void init()                            { initialize(output, exact(input));
						 level::fill(inplace(output | (input | true).domain()),  0);
						 level::fill(inplace(output | (input | false).domain()), max); }
	inline
	bool handles(const P& p) const         { return input(p) == false; }

	// end of requirements

	inline
	chamfer_t(const I_& input, const W_& win, unsigned max)
	  : input (input),
	    win (win),
	    max (max)
	{}
      };

      /// \internal Routines.

      template <typename I, typename W>
      inline
      mln_ch_value(I, unsigned)
	chamfer_(const Image<I>& input_, const W& w_win_,
		 unsigned max = mln_max(unsigned))
      {
	typedef chamfer_t<I, W> F;

	F f(exact(input_), exact(w_win_), max);
	canvas::chamfer<F> run(f);
	return f.output;
      }

    } // end of namespace mln::geom::impl

#endif // !MLN_INCLUDE_ONLY


    // Facade.

    template <typename I, typename W>
    mln_ch_value(I, unsigned)
      chamfer(const Image<I>& input_, const W& w_win_,
	      unsigned max = mln_max(unsigned))
    {
      return impl::chamfer_(exact (input_), exact(w_win_), max);
    }


  } // end of namespace mln::geom

}  // end of namespace mln

#endif // !MLN_GEOM_CHAMFER_HH
