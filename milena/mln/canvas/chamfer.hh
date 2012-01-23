// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CANVAS_CHAMFER_HH
# define MLN_CANVAS_CHAMFER_HH

/*! \file
 *
 * \brief Apply chamfer algorithm to a binary image.
 */

# include <mln/core/internal/image_morpher.hh>
# include <mln/win/sym.hh>

namespace mln
{
  namespace canvas
  {

    /*!
     * \brief Compute chamfer distance.
     *
     * \ingroup modcanvas
     */
    template <typename F>
    struct chamfer
    {
      typedef typename F::I I;
      typedef typename F::W W;
      typedef mln_psite(I) point;

      F& f;

      chamfer(F& f);

      void run();
    };

# ifndef MLN_INCLUDE_ONLY

    template<typename F>
    inline
    chamfer<F>::chamfer(F& f)
      : f(f)
    {
      run();
    }

    template<typename F>
    inline
    void
    chamfer<F>::run()
    {

      /// Init.
      {
	f.init();
      }

      /// Fwd pass.
      {
	mln_fwd_piter(I) p(f.input.domain());
	mln_qiter(W) q(f.win, p);

	for_all(p) if (f.handles (p))
	  for_all(q) if (f.input.domain().has(q))
	    if (f.output(q) != f.max
		&& f.output(q) + q.w() < f.output(p))
	      f.output(p) = f.output(q) + q.w();
      }

      /// Bkd pass.
      {
	W w_win_b = win::sym(f.win);

	mln_bkd_piter(I) p(f.input.domain());
	mln_qiter(W) q(w_win_b, p);

	for_all(p) if (f.handles (p))
	  for_all(q) if (f.input.domain().has(q))
	    if (f.output(q) != f.max
		&& f.output(q) + q.w() < f.output(p))
	      f.output(p) = f.output(q) + q.w();
	f.status = true;
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of mln::canvas

} // end of mln

#endif // ! MLN_CANVAS_CHAMFER_HH
