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

#ifndef MLN_CANVAS_BROWSING_SNAKE_FWD_HH
# define MLN_CANVAS_BROWSING_SNAKE_FWD_HH

/*! \file mln/canvas/browsing/snake_fwd.hh
 *
 * \brief Browsing in a snake-way, forward.
 */

# include <mln/core/concept/browsing.hh>
# include <mln/geom/size2d.hh>


namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /*!
       * \brief Browsing in a snake-way, forward.
       *
       * This canvas browse all the point of an image 'input' like
       * this :
       *
       *     ------->
       *     <------'
       *     '------>
       *
       * The fonctor should provide (In addition to 'input') four
       * methods :
       *
       *   - init() : Will be called at the beginning.
       *   - down() : Will be called after each moving down. (will
       *   also be called once at the first point).
       *   - fwd()  : Will be called after each moving right.
       *   - bwd()  : Will ba called after each moving left.
       *
       * This methods should acces to the current working point 'p'
       * also provided by the functor.
       *
       * Warning: This canvas works only on 2D.
       *
       * F shall feature: \n
       * { \n
       *   --- as attributes: \n
       *   input; \n
       *   p; \n
       *   --- as methods: \n
       *   void init(); \n
       *   void down(); \n
       *   void fwd(); \n
       *   void bkd(); \n
       * } \n
       *
       */

      struct snake_fwd_t : public Browsing< snake_fwd_t >
      {
	template <typename F>
	void operator()(F& f) const;
      }

      snake_fwd;



# ifndef MLN_INCLUDE_ONLY

      template <typename F>
      void
      snake_fwd_t::operator()(F& f) const
      {
	// FIXME: Check the dimension (2D) or generalize.
	trace::entering("canvas::browsing::snake_fwd");
	mln_precondition(f.input.has_data());
	int
	  min_row = geom::min_row(f.input), max_row = geom::max_row(f.input),
	  min_col = geom::min_col(f.input), max_col = geom::max_col(f.input);

	// p
	f.p = f.input.bbox().pmin();
	int& row = f.p.row();
	int& col = f.p.col();

	// initialization
	trace::entering("canvas::browsing::snake_fwd::init");
	f.init();
	trace::exiting("canvas::browsing::snake_fwd::init");

	bool fwd = true;
	for (row = min_row; row <= max_row; ++row)
	  // FIXME: Add "if (f.input.has(p))"?
	  {
	    // go down
	    trace::entering("canvas::browsing::snake_fwd::init");
	    f.down();
	    trace::exiting("canvas::browsing::snake_fwd::init");

	    if (fwd)
	      // browse line fwd
	      while (col < max_col)
		{
		  ++col;
		  trace::entering("canvas::browsing::snake_fwd::fwd");
		  f.fwd();
		  trace::exiting("canvas::browsing::snake_fwd::fwd");
		}
	    else
	      // browse line bkd
	      while (col > min_col)
		{
		  --col;
		  trace::entering("canvas::browsing::snake_fwd");
		  f.bkd();
		  trace::exiting("canvas::browsing::snake_fwd");
		}

	    // change browsing
	    fwd = ! fwd;
	  }
	trace::exiting("canvas::browsing::snake_fwd");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_SNAKE_FWD_HH
