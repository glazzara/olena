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

#ifndef MLN_CANVAS_DIRBROWSING_HH
# define MLN_CANVAS_DIRBROWSING_HH

/*! \file mln/canvas/dirbrowsing.hh
 *
 * \brief Directional browsing of an image.
 */

namespace mln
{

  namespace canvas
  {

    /*! FIXME : DOC
     * F shall features : \n
     * { \n
     * --- as types: \n
     *   point; \n
     * --- as attributes: \n
     *   dim; \n
     *   dir; // and test dir < dim \n
     *   input; \n
     *   win; \n
     * --- as methods: \n
     *   void init(); \n
     *   void process()
     * } \n
     *
     */
    template <typename F>
    void dirbrowsing(F& f);

# ifndef MLN_INCLUDE_ONLY

    template <typename F>
    void dirbrowsing(F& f)
    {
      mln_point(F)
	pmin = f.input.domain().pmin(),
	pmax = f.input.domain().pmax();

      mln_point(F::point) p = pmin;

      f.init();

      do
      {
	f.process(p);

	for (int c = F::dim - 1; c >= 0; --c)
	{
	  if (c == int(f.dir))
	    continue;
	  if (p[c] != pmax[c])
	  {
	    ++p[c];
	    break;
	  }
	  p[c] = pmin[c];
	}
      } while (p != pmin);
    }

# endif // ! MLN_INCLUDE_ONLY

  } //end of namespace mln::canvas

} // end of namespace mln

#endif // ! MLN_CANVAS_DIRBROWSING_HH
