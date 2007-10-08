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

/*! \file mln/morpho/erosion.hh
 *
 * \brief Morphological erosion.
 *
 */

# include <mln/morpho/includes.hh>

// Specializations are in:
# include <mln/morpho/erosion.spe.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological erosion.
     *
     * \todo Overload erosion_wrt_win for hline and vline and for fast
     * images.
     */
    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {


      namespace generic
      {
	// On function.

	template <typename I, typename W, typename O, typename A>
	void erosion_on_function(const I& input, const W& win, O& output, A& min)
	{
	  trace::entering("morpho::impl::generic::erosion_on_function");
	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(win, p);
	  for_all(p)
	    {
	      min.init();
	      for_all(q) if (input.has(q))
		min.take(input(q));
	      output(p) = min.to_result();
	    }
	  trace::exiting("morpho::impl::generic::erosion_on_function");
	}

	// On set.

	template <typename I, typename W, typename O>
	void erosion_on_set(const I& input, const W& win, O& output)
	{
	  trace::entering("morpho::impl::generic::erosion_on_set");
	  level::fill(output, input);

	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(win, p);
	  for_all(p)
	    if (input(p))
	      for_all(q) if (input.has(q))
		if (! input(q))
		{
		  output(p) = false;
		  break;
		}
	  trace::exiting("morpho::impl::generic::erosion_on_set");
	}

      } // end of namespace mln::morpho::impl::generic

    } // end of namespace mln::morpho::impl



    // Facade.

    template <typename I, typename W, typename O>
    void erosion(const Image<I>& input, const Window<W>& win, Image<O>& output)
    {
      trace::entering("morpho::erosion");
      mln_precondition(exact(output).domain() == exact(input).domain());
      mln_precondition(! exact(win).is_empty());

      impl::erosion_wrt_win(input, exact(win), output);

      if (exact(win).is_centered())
	mln_postcondition(output <= input);
      trace::exiting("morpho::erosion");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_EROSION_HH
