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

#ifndef MLN_MORPHO_DILATION_HH
# define MLN_MORPHO_DILATION_HH

/// \file
///
/// Morphological dilation.
///
/// \todo The overloads are hidden and I don't know why!

# include <mln/morpho/includes.hh>
# include <mln/morpho/general.hh>
# include <mln/accu/logic/lor.hh>
# include <mln/accu/logic/lor_basic.hh>
# include <mln/accu/stat/max.hh>
# include <mln/accu/stat/max_h.hh>


namespace mln
{

  namespace morpho
  {

    /*! \brief Morphological dilation.
     *
     * \ingroup mlnmorpho
     */
    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY


    struct dilation_op
    {

      template <typename I>
      mln_morpho_select_accu(I, logic::lor_basic, stat::max)
      accu(const Image<I>&) const
      {
	mln_morpho_select_accu(I, logic::lor_basic, stat::max) tmp;
	return tmp;
      }

      template <typename I>
      mln_morpho_select_accu(I, logic::lor, stat::max_h)
      accu_incr(const Image<I>&) const
      {
	mln_morpho_select_accu(I, logic::lor, stat::max_h) tmp;
	return tmp;
      }

      template <typename I>
      mln_value(I) neutral(const Image<I>&) const
      {
	return internal::neutral<I>::infimum();
      }

    };


    namespace impl
    {


      // On set with centered window (overloads).

      template <typename I, typename W>
      mln_concrete(I)
      general_on_set_centered(const dilation_op&,
			      const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered__dilation");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);

	extension::adjust_fill(input, win, false);

	O output;
	output = duplicate(input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  if (input(p) == false)
	    for_all(q) if (input.has(q))
	      if (input(q) == true)
	      {
		output(p) = true;
		break;
	      }

	trace::exiting("morpho::impl::general_on_set_centered__dilation");
	return output;
      }


      template <typename I, typename W>
      mln_concrete(I)
      general_on_set_centered_fastest(const dilation_op&,
				      const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered_fastest__dilation");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);

	extension::adjust_fill(input, win, false);

	O output;
	output = duplicate(input);

	mln_pixter(const I) p(input);
	mln_qixter(const I, W) q(p, win);
	mln_pixter(O) p_out(output);
	for_all_2(p, p_out)
	  if (p.val() == false)
	    for_all(q)
	      if (q.val() == true)
	      {
		p_out.val() = true;
		break;
	      }

	trace::exiting("morpho::impl::general_on_set_centered_fastest__dilation");
	return output;
      }

    } // end of namespace morpho::impl


    template <typename I, typename W>
    inline
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::dilation");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = general(dilation_op(), input, win);

      if (exact(win).is_centered())
	mln_postcondition(output >= input);

      trace::exiting("morpho::dilation");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_DILATION_HH
