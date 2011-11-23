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

#ifndef MLN_CANVAS_BROWSING_DIR_STRUCT_ELT_INCR_UPDATE_HH
# define MLN_CANVAS_BROWSING_DIR_STRUCT_ELT_INCR_UPDATE_HH

/// \file
///
/// Directional browsing of an image with structuring element.

# include <mln/core/concept/browsing.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Browsing in a certain direction with a segment.
      /*!
       * This canvas browse all the point of an image 'input' of type
       * 'I', of dimension 'dim' in the direction 'dir' with
       * considering weigh the 'length' nearest points.
       *
       * The functor should provide (In addition to 'input', 'I',
       * 'dim', 'dir' and 'length') six methods :
       *
       *   - init()         : Will be called at the beginning.
       *   - init_line()    : Will be called at the beginning of each
       *                      line.
       *   - add_point(q)   : Will be called for taking the new point
       *                      'q' into account.
       *   - remove_point(q): Will be called for untaking the new point
       *                      'q' into account.
       *   - next()         : Will be called at each point 'p' (also
       *                      provided by the functor).
       *   - final()        : Will be called at the end.
       *
       * F shall features : \n
       * { \n
       * --- as types: \n
       *   I; \n
       * --- as attributes: \n
       *   dim; \n
       *   dir; // and test dir < dim \n
       *   input; \n
       *   p; \n
       *   length; \n
       * --- as methods: \n
       *   void init(); \n
       *   void init_line(); \n
       *   void add_point(q) \n
       *   void remove_point(q) \n
       *   void next(); \n
       *   void final(); \n
       * } \n
       *
       */
      struct dir_struct_elt_incr_update_t : public Browsing< dir_struct_elt_incr_update_t >
      {
	// This default constructor is needed for compilation with gcc
	// 4.6.0, gcc 4.6.1 and Clang.
	dir_struct_elt_incr_update_t();

	template <typename F>
	void operator()(F& f) const;
      };

      extern const dir_struct_elt_incr_update_t dir_struct_elt_incr_update;

# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

      const dir_struct_elt_incr_update_t dir_struct_elt_incr_update;

#  endif // ! MLN_WO_GLOBAL_VARS

      inline
      dir_struct_elt_incr_update_t::dir_struct_elt_incr_update_t()
      {
      }

      template <typename F>
      inline
      void
      dir_struct_elt_incr_update_t::operator()(F& f) const
      {
	trace::entering("canvas::browsing::dir_struct_elt_incr_update");
	mln_precondition(f.dir < f.dim);
	typedef typename F::I I;

	const mln_psite(I)
	  pmin = f.input.domain().pmin(),
	  pmax = f.input.domain().pmax();

	typedef mln_deduce(I, site, coord) C;
	const C
	  pmin_dir = pmin[f.dir],
	  pmax_dir = pmax[f.dir],
	  pmin_dir_plus_half_length  = static_cast<C>(pmin_dir + f.length / 2),
	  pmax_dir_minus_half_length = static_cast<C>(pmax_dir - f.length / 2);

	mln_psite(I) pt, pu;

	def::coord&
	  ct = pt[f.dir],
	  cu = pu[f.dir],
	  p_dir = f.p[f.dir];

	f.p = pmin;

	f.init();

	do
	{
	  pt = f.p;
	  pu = f.p;

	  f.init_line();

	  // initialization (before first point of the line)
	  std::cout << "init" << std::endl;
	  for (ct = pmin_dir; ct < pmin_dir_plus_half_length; ++ ct)
	    if (f.input.has(pt))
	      {
		std::cout << '+' << pt << ' ';
		f.add_point(pt);
	      }

	  // left columns (just take new points)
	  std::cout << "left" << std::endl;
	  for (p_dir = pmin_dir; p_dir <= pmin_dir_plus_half_length; ++p_dir, ++ct)
	  {
	    if (f.input.has(pt))
	      {
		std::cout << '+' << pt << ' ';
		f.add_point(pt);
	      }
	    f.next();
	  }

	  // middle columns (both take and untake)
	  std::cout << "middle" << std::endl;
	  cu = pmin_dir;
	  for (; p_dir <= pmax_dir_minus_half_length; ++cu, ++p_dir, ++ct)
	  {
	    if (f.input.has(pt))
	      {
		std::cout << '+' << pt << ' ';
		f.add_point(pt);
	      }
	    if (f.input.has(pu))
	      {
		std::cout << '-' << pu << ' ';
		f.remove_point(pu);
	      }
	    f.next();
	  }

	  // right columns (now just untake old points)
	  std::cout << "right" << std::endl;
	  for (; p_dir <= pmax_dir; ++cu, ++p_dir)
	  {
	    if (f.input.has(pu))
	      {
		std::cout << '-' << pu << ' ';
		f.remove_point(pu);
	      }
	    f.next();
	  }

	  p_dir = pmin_dir;

	  for (int c = F::dim - 1; c >= 0; --c)
	  {
	    if (c == int(f.dir))
	      continue;
	    if (f.p[c] != pmax[c])
	    {
	      ++f.p[c];
	      break;
	    }
	    f.p[c] = pmin[c];
	  }
	} while (f.p != pmin);

	f.final();
	trace::exiting("canvas::browsing::dir_struct_elt_incr_update");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln

#endif // ! MLN_CANVAS_BROWSING_DIR_STRUCT_ELT_INCR_UPDATE_HH
