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

#ifndef MLN_MORPHO_EROSION_HH
# define MLN_MORPHO_EROSION_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
// FIXME: # include <mln/border/assign.hh>
# include <mln/value/props.hh>


namespace mln
{

  namespace morpho
  {

    template <typename I, typename W>
    I erosion(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename W>
      I erosion(const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);

	typedef mln_value(I) value;

	I output(input.domain());
	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  {
	    value v = mln_max(value);
	    for_all(q) if (input.owns_(q))
	      {
		if (input(q) < v)
		  v = input(q);
	      }
	    output(p) = v;
	  }
	return output;
      }

      // ...

    } // end of namespace mln::morpho::impl


    // facade

    template <typename I, typename W>
    I erosion(const Image<I>& input, const Window<W>& win)
    {
      return impl::erosion(exact(input), exact(win)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_EROSION_HH
