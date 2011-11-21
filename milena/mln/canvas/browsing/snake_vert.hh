// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_CANVAS_BROWSING_SNAKE_VERT_HH
# define MLN_CANVAS_BROWSING_SNAKE_VERT_HH

/// \file
///
/// Browsing in a snake-way, forward.

# include <mln/core/concept/browsing.hh>
# include <mln/geom/size2d.hh>


namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Browsing in a snake-way, forward.
      /*!
       * This canvas browse all the point of an image 'input' like
       * this :
       *
       *     |  /\ |
       *     |  |  |
       *     \/ |  \/
       *
       * The fonctor should provide (In addition to 'input') four
       * methods :
       *
       *   - init() : Will be called at the beginning.
       *   - down() : Will be called after each moving down.
       *   - up()  : Will ba called after each moving up.
       *   - fwd()  : Will be called after each moving right. (will
       *   also be called once at the first point).
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
       *   void up(); \n
       *   void fwd(); \n
       * } \n
       *
       */

      struct snake_vert_t : public Browsing< snake_vert_t >
      {
	template <typename F>
	void operator()(F& f) const;
      };

      extern const snake_vert_t snake_vert;

# ifndef MLN_INCLUDE_ONLY

      const snake_vert_t snake_vert = snake_vert_t();

      template <typename F>
      inline
      void
      snake_vert_t::operator()(F& f) const
      {
	// Fixme: check the dimension of the input
//	mlc_equal(mln_trait_image_dimension(I)(),
//		  trait::image::dimension::two_d)::check();
	trace::entering("canvas::browsing::snake_vert");
	mln_precondition(f.input.is_valid());
	int
	  min_row = geom::min_row(f.input), max_row = geom::max_row(f.input),
	  min_col = geom::min_col(f.input), max_col = geom::max_col(f.input);

	// p
	f.p = f.input.bbox().pmin();
	def::coord& row = f.p.row();
	def::coord& col = f.p.col();

	// initialization
	f.init();

	bool down = true;
	for (col = min_col; col <= max_col; ++col)
	  // FIXME: Add "if (f.input.has(p))"?
	  {
	    // go fwd
	    f.fwd();

	    if (down)
	      // browse col down.
	      while (row < max_row)
		{
		  ++row;
		  f.down();
		}
	    else
	      // browse col up.
	      while (row > min_row)
		{
		  --row;
		  f.up();
		}

	    // change browsing
	    down = ! down;
	  }
	trace::exiting("canvas::browsing::snake_vert");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_SNAKE_VERT_HH
