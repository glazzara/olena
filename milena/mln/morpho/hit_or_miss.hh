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

#ifndef MLN_MORPHO_HIT_OR_MISS_HH
# define MLN_MORPHO_HIT_OR_MISS_HH

/*! \file mln/morpho/hit_or_miss.hh
 *
 * \brief Morphological hit-or-miss.
 *
 * \todo Save memory.
 */

# include <mln/morpho/includes.hh>
# include <mln/pw/all.hh>
# include <mln/fun/p2v/ternary.hh>


namespace mln
{

  namespace morpho
  {


    bool constrained_hit_or_miss = true;


    /*! Morphological hit-or-miss.
     *
     * This operator is HMT_(Bh,Bm) = e_Bh /\ (e_Bm o C). 
     */
    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss(const Image<I>& input,
		     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
		     Image<O>& output);

    /*! Morphological hit-or-miss opening.
     *
     * This operator is HMTope_(Bh,Bm) = d_(-Bh) o HMT_(Bh,Bm).
     */
    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_opening(const Image<I>& input,
			     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			     Image<O>& output);

    /*! Morphological hit-or-miss opening of the background.
     *
     * This operator is HMTopeBG = HMTope_(Bm,Bh) o C = d_(-Bm) o HMT_(Bh,Bm).
     */
    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_background_opening(const Image<I>& input,
					const Window<Wh>& win_hit, const Window<Wm>& win_miss,
					Image<O>& output);

    /*! Morphological hit-or-miss closing.
     *
     * This operator is C o HMTope o C.
     */
    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_closing(const Image<I>& input,
			     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			     Image<O>& output);

    /*! Morphological hit-or-miss closing of the background.
     *
     * This operator is C o HMTopeBG o C.
     */
    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_background_closing(const Image<I>& input,
					const Window<Wh>& win_hit, const Window<Wm>& win_miss,
					Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Preconditions.

      template <typename I, typename Wh, typename Wm, typename O>
      void hit_or_miss_preconditions_(const Image<I>& input,
				      const Window<Wh>& win_hit, const Window<Wm>& win_miss,
				      Image<O>& output)
      {
	mln_precondition(exact(output).domain() == exact(input).domain());
	mln_precondition(set::inter(exact(win_hit), exact(win_miss)).is_empty());
      }

      // On sets.

      template <typename I, typename Wh, typename Wm, typename O>
      void hit_or_miss_(trait::kind::logic, // binary => morphology on sets
			const Image<I>& input,
			const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			Image<O>& output)
      {
	erosion(input, win_hit, output); // output = ero(input)_hit

	O temp_1(exact(input).domain());
	complementation(input, temp_1); // temp1 = C(input)

	O temp_2(exact(input).domain());
	erosion(temp_1, win_miss, temp_2); // temp_2 = ero(C(input))_miss

	logical::and_inplace(output, temp_2); // output = ero(input)_hit /\ ero(C(input))_miss
      }

      // On functions.

      template <typename K,
		typename I, typename Wh, typename Wm, typename O>
      void hit_or_miss_(K, // otherwise => morphology on functions
			const Image<I>& input,
			const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			Image<O>& output)
      {
	typedef mln_value(O)  V;

	if (constrained_hit_or_miss) // CHMT.
	  {
	    if (exact(win_hit).is_centered())
	      {
		O ero_fg(exact(input).domain()), dil_bg(exact(input).domain());
		erosion(input, win_hit,  ero_fg);
		dilation(input, win_miss, dil_bg);
		level::fill(output,
			    fun::p2v::ternary(pw::value(input) == pw::value(ero_fg)
					        && pw::value(dil_bg) < pw::value(input),
					      pw::value(input) - pw::value(dil_bg),
					      pw::cst(V::zero)));
	      }
	    else if (exact(win_miss).is_centered())
	      {
		O ero_bg(exact(input).domain()), dil_fg(exact(input).domain());
		erosion(input, win_miss, ero_bg);
		dilation(input, win_hit, dil_fg);
		level::fill(output,
			    fun::p2v::ternary(pw::value(input) == pw::value(dil_fg)
					        && pw::value(ero_bg) > pw::value(input),
					      pw::value(ero_bg) - pw::value(input),
					      pw::cst(V::zero)));
	      }
	    else
	      level::fill(output, V::zero);
	  }
	else // Unconstrained: UHMT.
	  {
	    O ero(exact(input).domain()), dil(exact(input).domain());
	    erosion(input, win_hit, ero);
	    dilation(input, win_miss, dil);
	    level::fill(output,
			fun::p2v::ternary(pw::value(dil) < pw::value(ero),
					  pw::value(ero) - pw::value(dil),
					  pw::cst(V::zero)));
	  }
      }

    } // end of mln::morpho::impl


    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss(const Image<I>& input,
		     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
		     Image<O>& output)
    {
      impl::hit_or_miss_preconditions_(input, win_hit, win_miss, output);
      impl::hit_or_miss_(mln_value_kind(mln_value(I))(), input, win_hit, win_miss, output);
    }

    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_opening(const Image<I>& input,
			     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			     Image<O>& output)
    {
      impl::hit_or_miss_preconditions_(input, win_hit, win_miss, output);

      O temp(exact(input).domain());
      hit_or_miss(input, win_hit, win_miss, temp);
      dilation(temp, geom::sym(win_hit), output);
      // FIXME: Postcondition.
    }

    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_background_opening(const Image<I>& input,
					const Window<Wh>& win_hit, const Window<Wm>& win_miss,
					Image<O>& output)
    {
      impl::hit_or_miss_preconditions_(input, win_hit, win_miss, output);
      O temp(exact(input).domain());
      complementation(input, temp);
      hit_or_miss_opening(temp, win_miss, win_hit, output);
#  ifndef NDEBUG
      {
	O temp(exact(input).domain());
	hit_or_miss(input, win_hit, win_miss, temp);
	O output_(exact(input).domain());
	dilation(temp, geom::sym(win_miss), output_);
	mln_postcondition(output_ == output);
      }
#  endif // ! NDEBUG
    }

    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_closing(const Image<I>& input,
			     const Window<Wh>& win_hit, const Window<Wm>& win_miss,
			     Image<O>& output)
    {
      impl::hit_or_miss_preconditions_(input, win_hit, win_miss, output);
      I temp(input.domain());
      complementation(input, temp);
      hit_or_miss_opening(temp, win_hit, win_miss, output);
      complementation_inplace(output);
      // FIXME: Postcondition.
    }

    template <typename I, typename Wh, typename Wm, typename O>
    void hit_or_miss_background_closing(const Image<I>& input,
					const Window<Wh>& win_hit, const Window<Wm>& win_miss,
					Image<O>& output)
    {
      impl::hit_or_miss_preconditions_(input, win_hit, win_miss, output);
      hit_or_miss_closing(input, win_miss, win_hit, output);
#  ifndef NDEBUG
      {
	O temp(exact(input).domain());
	complementation(input, temp);
	O output_(exact(input).domain());
	hit_or_miss_background_opening(temp, win_hit, win_miss, output_);
	complementation_inplace(output_);
	mln_postcondition(output_ == output);
      }
#  endif // ! NDEBUG
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_HIT_OR_MISS_HH
