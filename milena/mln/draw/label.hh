// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_DRAW_LABEL_HH
# define MLN_DRAW_LABEL_HH

/*! \file mln/draw/label.hh
 *
 * \brief Draw a random color image from a label image.
 */

# include <mln/core/clone.hh>
# include <mln/core/concept/image.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>

# include <mln/trait/image_from_mesh.hh>
# include <mln/core/image_if_value.hh>
# include <mln/value/rgb8.hh>
# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/metal/is_not.hh>
# include <mln/core/image_if_value.hh>
# include <mln/debug/println.hh>

namespace mln
{

  namespace draw
  {

    /*! Draw a new image from a data (int, unsigned, int_u8, etc...) label image.
     *
     * \param[in] input The input image.
     * \param[in] background The value to fill background output.
     *
     * \return A rgb8 image.
     *
     * \pre \p input has to be initialized.
     *
     */
    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    label(Image<I>& input, const value::rgb8& background);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    typename trait::image_from_mesh < mln_mesh(I), value::rgb8 >::ret
    label(Image<I>& input, const value::rgb8& background)
    {
      typedef mln_value(I) V;
      typedef mln_ch_value(I, value::rgb8) O;
      typedef const mln::pset_if<mln_pset(I), mln::fun::eq_p2b_expr_<mln::pw::value_<I>, mln::pw::cst_<V> > > F;

      I in = exact(input);
      mln_precondition(in.has_data());
      I ref = clone(in);
      O out (in.domain ());

      level::fill(out, background);
      mln_piter(I) p (out.domain());
      for_all (p)
      {
	if (ref(p) == 0)
	  continue;
	value::rgb8 color;
	while (1)
	  {
	    int r = rand() % 256;
	    int g = rand() % 256;
	    int b = rand() % 256;
	    if (r + g + b < 50)
	      continue;
	    color = value::rgb8(r, g, b);
	    break;
	  }
	V val = ref(p);
 	image_if_value<I> ima_if = ref | val;
	mln_piter(F) l (ima_if.domain());
	for_all (l)
	  out(l) = color;
 	level::fill(ima_if, 0);
      }
      return out;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::draw

} // end of namespace mln


#endif // ! MLN_DRAW_LINE_HH
