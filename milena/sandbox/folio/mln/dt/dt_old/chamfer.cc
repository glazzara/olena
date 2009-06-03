// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DT_CHAMFER_HH
# define MLN_DT_CHAMFER_HH

# include <mln/core/concept/image.hh>
# include <mln/make/w_window.hh>

namespace mln
{

  namespace dt
  {

    /*! Distance tranform by chamfer application.
     *
     * \param[in]  input_   The input image.
     * \param[in]  chamfer  The chamfer window to use for distance calcul.
     * \return              A pair (distance map, nearest point map).
     *
     * \pre \p img has to be initialized.
     */
    template<typename I, typename T>
    std::pair<mln_ch_value(I, T), mln_ch_value(I, mln_point(I))>
    chamfer(const Image<I>& input_,
	    w_window<mln_dpoint(I), T> chamfer);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      /*! Computes a pass of the chamfer DT algorithm.
       *
       * \param[in]  p              Iterator on the input image to use.
       * \param[in]  chamfer        The chamfer window to use for distance calcul.
       * \param[in]  input          The input image.
       * \param[out] outputDistance The distance map updated.
       * \param[out] outputnearest  The nearest points map updated.
       */
      template<typename Q, typename I, typename T>
      inline
      void
      chamfer_pass(const w_window<mln_dpoint(I), T> chamfer,
		   const I& input,
		   mln_ch_value(I, T)& outputDistance,
		   mln_ch_value(I, mln_point(I))& outputNearest)
      {
	typedef w_window<mln_dpoint(I), T> W;

	Q p(input.domain());
	mln_qiter(W) q(chamfer, p);
	for_all(p)
	{
	  std::pair<T, mln_point(I)> min(mln_max(T), p);

	  for_all(q)
	    if (input.has(q) && outputDistance(q) != mln_max(T))
	    {
	      T v = outputDistance(q) + q.w();

	      if (v < min.first)
	      {
		min.first = v;
		min.second = outputNearest(q);
	      }
	    }

	  if (min.first < outputDistance(p))
	  {
	    outputDistance(p) = min.first;
	    outputNearest(p) = min.second;
	  }
	}
      }

    } // end of namespace mln::dt::impl



    // Facade.

    template<typename I, typename T>
    inline
    std::pair<mln_ch_value(I, T), mln_ch_value(I, mln_point(I))>
    chamfer(const Image<I>& input_,
	    w_window<mln_dpoint(I), T> chamfer)
    {
      typedef w_window<mln_dpoint(I), T> W;

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      mln_ch_value(I, T) outputDistance;
      initialize(outputDistance, input);

      mln_ch_value(I, mln_point(I)) outputNearest;
      initialize(outputNearest, input);

      // Initialization.
      {
	mln_fwd_piter(I) p(input.domain());
	for_all(p)
	{
	  outputDistance(p) = input(p) ? literal::zero : mln_max(T);
	  outputNearest(p) = p;
	}
      }

      // First pass.
      impl::chamfer_pass<mln_fwd_piter(I)>
	(chamfer, input, outputDistance, outputNearest);

      chamfer.sym();

      // Second pass.
      impl::chamfer_pass<mln_bkd_piter(I)>
	(chamfer, input, outputDistance, outputNearest);

      return std::pair<mln_ch_value(I, T), mln_ch_value(I, mln_point(I))>
	(outputDistance, outputNearest);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_CHAMFER_HH

#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/data/fill.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/stretch.hh>
#include <mln/value/int_u8.hh>

int main()
{
  using namespace mln;

  w_window2d_int chamfer = make::mk_chamfer_3x3_int<1, 2>();

  {
//     image2d<bool> ima(5,5);
//     bool vals[] = { 1, 1, 1, 0, 0,
// 		    0, 0, 1, 0, 0,
// 		    0, 0, 1, 0, 0,
// 		    0, 0, 0, 0, 0,
// 		    0, 0, 0, 0, 0 };

//     data::fill(ima, vals);
//     debug::println(ima);

//     std::pair<image2d<int>, image2d<mln_point_(image2d<bool>)> > out;
//     for (int i = 0; i < 999; ++i)
//       out = dt::chamfer(ima, chamfer);

//     std::cerr << "Distance:" << std::endl;
//     debug::println(out.first);
//     std::cerr << "PPP:" << std::endl;
//     debug::println(out.second);

  image2d<bool> ima = io::pbm::load("../../img/c01.pbm");

  std::pair<image2d<int>, image2d<mln_point_(image2d<bool>)> > out;
  out = dt::chamfer(ima, chamfer);

  image2d<value::int_u8> out2(out.first.domain());
  data::stretch(out.first, out2);

  io::pgm::save(out2, "out.pgm");

  }
}
