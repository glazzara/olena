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

#ifndef MLN_LEVEL_MEDIAN_HH
# define MLN_LEVEL_MEDIAN_HH

/*! \file mln/level/median.hh
 *
 * \brief Median filter on an image.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/window2d.hh>
# include <mln/accu/median.hh>


namespace mln
{

  namespace level
  {

    /*! FIXME: Median the image \p ima with the values of the image \p data.
     *
     * \param[in] input The image to be filtered.
     * \param[in] win The window.
     * \param[in,out] output The output image.
     *
     * \warning The definition domain of \p ima has to be included in
     * the one of \p data.
     *
     * \pre \p ima has to be initialized.
     *
     * \todo Test domain inclusion.
     */
    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template <typename I, typename W, typename O>
      void median(const I& input,
		  const W& win,
		  O& output)
      {
  	dpoint2d dp = mk_dpoint2d(0, 1);

	window2d
	  w_plus  = win - (win - dp),
	  w_minus = (win - dp) - win;
	
	accu::median_on<mln_value(I)> med;

	point2d p;
	mln_qiter(W) q(win, p);
	mln_qiter(W) q_plus(w_plus, p);
	mln_qiter(W) q_minus(w_minus, p);

	for (p.row() = input.domain().pmin().row();
	     p.row() <= input.domain().pmax().row();
	     ++p.row())
	  {
	    // init
	    med.init();
	    p.col() = input.domain().pmin().col();
	    for_all(q)
	      if (input.has(q))
		med.take(input(q));
	    output(p) = med;

	    // other col
	    for (++p.col();
		 p.col() <= input.domain().pmax().col();
		 ++p.col())
	      {
		for_all(q_plus)
		  if (input.has(q_plus))
		    med.take(input(q_plus));
		for_all(q_minus)
		  if (input.has(q_minus))
		    med.untake(input(q_minus));
		output(p) = med;
	      }
	  }
      }

    } // end of namespace mln::level::impl


    // facade

    template <typename I, typename W, typename O>
    void median(const Image<I>& input, const Window<W>& win,
		Image<O>& output)
    {
      impl::median(exact(input), exact(win), exact(output)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_MEDIAN_HH
