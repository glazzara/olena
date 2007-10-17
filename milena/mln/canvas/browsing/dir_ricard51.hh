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

#ifndef MLN_CANVAS_DIR_RICARD51_HH
# define MLN_CANVAS_DIR_RICARD51_HH

/*! \file mln/canvas/browsing/dir_ricard51.hh
 *
 * \brief Dir_Ricard51 browsing of an image.
 */

# include <mln/core/concept/browsing.hh>
# include <mln/core/concept/image.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {
      
      /*! FIXME : DOC
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
       *   void add_point(p) \n
       *   void remove_point(p) \n
       *   void next(); \n
       *   void final(); \n
       * } \n
       *
       */
      struct dir_ricard51_t : public Browsing< dir_ricard51_t >
      {
	template <typename F>
	void operator()(F& f) const;
      }

      dir_ricard51;

# ifndef MLN_INCLUDE_ONLY

      template <typename F>
      void
      dir_ricard51_t::operator()(F& f) const
      {
	mln_precondition(f.dir < f.dim);
	typedef typename F::I I;

	const mln_point(I)
	  pmin = f.input.domain().pmin(),
	  pmax = f.input.domain().pmax();

	const mln_coord(I)
	  pmin_dir = pmin[f.dir],
	  pmax_dir = pmax[f.dir],
	  pmin_dir_plus_half_length = pmin_dir + f.length / 2,
	  pmax_dir_minus_half_length = pmax_dir - f.length / 2;

	mln_point(I) pt, pu;
	
	typedef mln_coord(I)& coord_ref;
	coord_ref
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
	  for (ct = pmin_dir; ct < pmin_dir_plus_half_length; ++ ct)
	    if (f.input.has(pt))
	      f.add_point(pt);

	  // left columns (just take new points)
	  for (p_dir = pmin_dir; p_dir <= pmin_dir_plus_half_length; ++p_dir, ++ct)
	  {
	    if (f.input.has(pt))
	      f.add_point(pt);
	    f.next();
	  }

	  // middle columns (both take and untake)
	  cu = pmin_dir;
	  for (; p_dir <= pmax_dir_minus_half_length; ++cu, ++p_dir, ++ct)
	  {
	    if (f.input.has(pt))
	      f.add_point(pt);
	    if (f.input.has(pu))
	      f.remove_point(pu);
	    f.next();
	  }

	  // right columns (now just untake old points)
	  for (; p_dir <= pmax_dir; ++cu, ++p_dir)
	  {
	    if (f.input.has(pu))
	      f.remove_point(pu);
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
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln

#endif // ! MLN_CANVAS_DIR_RICARD51_HH
