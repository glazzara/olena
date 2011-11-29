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

#ifndef MLN_MORPHO_HIT_OR_MISS_HH
# define MLN_MORPHO_HIT_OR_MISS_HH

/// \file
///
/// Morphological hit-or-miss.
///
/// \todo Save memory.

# include <mln/morpho/includes.hh>
# include <mln/pw/all.hh>
# include <mln/fun/p2v/ternary.hh>
# include <mln/fun/cast.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace morpho
  {


    extern bool constrained_hit_or_miss;


    /// Morphological hit-or-miss.
    /*!
     * This operator is HMT_(Bh,Bm) = e_Bh /\ (e_Bm o C).
     */
    template <typename I, typename Wh, typename Wm>
    mln_concrete(I)
    hit_or_miss(const Image<I>& input,
		const Window<Wh>& win_hit, const Window<Wm>& win_miss);


    /// Morphological hit-or-miss opening.
    /*!
     * This operator is HMTope_(Bh,Bm) = d_(-Bh) o HMT_(Bh,Bm).
     */
    template <typename I, typename Wh, typename Wm>
    mln_concrete(I)
    hit_or_miss_opening(const Image<I>& input,
			const Window<Wh>& win_hit, const Window<Wm>& win_miss);


    /// Morphological hit-or-miss opening of the background.
    /*!
     * This operator is HMTopeBG = HMTope_(Bm,Bh) o C = d_(-Bm) o HMT_(Bh,Bm).
     */
    template <typename I, typename Wh, typename Wm>
    mln_concrete(I)
    hit_or_miss_background_opening(const Image<I>& input,
				   const Window<Wh>& win_hit, const Window<Wm>& win_miss);


    /// Morphological hit-or-miss closing.
    /*!
     * This operator is C o HMTope o C.
     */
    template <typename I, typename Wh, typename Wm>
    mln_concrete(I)
    hit_or_miss_closing(const Image<I>& input,
			const Window<Wh>& win_hit, const Window<Wm>& win_miss);


    /// Morphological hit-or-miss closing of the background.
    /*!
     * This operator is C o HMTopeBG o C.
     */
    template <typename I, typename Wh, typename Wm>
    mln_concrete(I)
    hit_or_miss_background_closing(const Image<I>& input,
				   const Window<Wh>& win_hit, const Window<Wm>& win_miss);



# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    bool constrained_hit_or_miss = true;

#  endif // ! MLN_WO_GLOBAL_VARS

    namespace internal
    {

      template <typename I, typename Wh, typename Wm>
      inline
      void
      hit_or_miss_tests(const Image<I>&   input_,
			const Window<Wh>& win_hit_,
			const Window<Wm>& win_miss_)
      {
	const I&  input    = exact(input_);
	const Wh& win_hit  = exact(win_hit_);
	const Wm& win_miss = exact(win_miss_);

	// Tests.
	mln_precondition(input.is_valid());
	mln_precondition((win_hit && win_miss).is_empty());

	// Avoid warnings.
	(void) input;
	(void) win_hit;
	(void) win_miss;
      }

    } // end of namespace mln::morpho::internal


    namespace impl
    {

      // On sets.

      template <typename I, typename Wh, typename Wm>
      inline
      mln_concrete(I)
      hit_or_miss_logic(const Image<I>& input,
			const Window<Wh>& win_hit,
			const Window<Wm>& win_miss)
      {
	trace::entering("morpho::impl::hit_or_miss_logic");
	internal::hit_or_miss_tests(input, win_hit, win_miss);

	mln_concrete(I) output = logical::and_(erosion(input, win_hit),
					       erosion(complementation(input),
						       win_miss));

	trace::exiting("morpho::impl::hit_or_miss_logic");
	return output;
      }



      namespace generic
      {

	// On functions.

	template <typename I, typename Wh, typename Wm>
	inline
	mln_concrete(I)
	hit_or_miss(const Image<I>& input_,
		    const Window<Wh>& win_hit_,
		    const Window<Wm>& win_miss_)
	{
	  trace::entering("morpho::impl::generic::hit_or_miss");
	  internal::hit_or_miss_tests(input_, win_hit_, win_miss_);

	  const I& input = exact(input_);
	  const Wh& win_hit = exact(win_hit_);
	  const Wm& win_miss = exact(win_miss_);

	  typedef mln_value(I) V;
	  mln_value(I) zero_V = literal::zero;

	  mln_concrete(I) output;
	  initialize(output, input);

	  if (constrained_hit_or_miss) // CHMT.
	  {
	    if (win_hit.is_centered())
	    {
	      mln_concrete(I)
		ero_fg = erosion(input, win_hit),
		dil_bg = dilation(input, win_miss);
	      data::fill(output,
			  fun::p2v::ternary(pw::value(input) == pw::value(ero_fg)
			    && pw::value(dil_bg) < pw::value(input),
			    fun::cast<V>(pw::value(input) - pw::value(dil_bg)),
			    pw::cst(zero_V)));
	    }
	    else if (win_miss.is_centered())
	    {
	      mln_concrete(I)
		ero_bg = erosion(input, win_miss),
		dil_fg = dilation(input, win_hit);
	      data::fill(output,
			  fun::p2v::ternary(pw::value(input) == pw::value(dil_fg)
			    && pw::value(ero_bg) > pw::value(input),
			    fun::cast<V>(pw::value(ero_bg) - pw::value(input)),
			    pw::cst(zero_V)));
	    }
	    else
	      data::fill(output, zero_V);
	  }
	  else // Unconstrained: UHMT.
	  {
	    mln_concrete(I)
	      ero = erosion(input, win_hit),
	      dil = dilation(input, win_miss);
	    data::fill(output,
			fun::p2v::ternary(pw::value(dil) < pw::value(ero),
			  fun::cast<V>(pw::value(ero) - pw::value(dil)),
			  pw::cst(zero_V)));
	  }

	  trace::exiting("morpho::impl::generic::hit_or_miss");
	  return output;
	}

      } // end of namespace mln::morpho::impl::generic

    } // end of mln::morpho::impl


    namespace internal
    {

      template <typename I, typename Wh, typename Wm>
      inline
      mln_concrete(I)
      hit_or_miss_dispatch(trait::image::kind::any,
			   const Image<I>& input,
			   const Window<Wh>& win_hit,
			   const Window<Wm>& win_miss)
      {
	return impl::generic::hit_or_miss(input, win_hit, win_miss);
      }

      template <typename I, typename Wh, typename Wm>
      inline
      mln_concrete(I)
      hit_or_miss_dispatch(trait::image::kind::logic,
			   const Image<I>& input,
			   const Window<Wh>& win_hit,
			   const Window<Wm>& win_miss)
      {
	return impl::hit_or_miss_logic(input, win_hit, win_miss);
      }


      template <typename I, typename Wh, typename Wm>
      inline
      mln_concrete(I)
      hit_or_miss_dispatch(const Image<I>& input,
			   const Window<Wh>& win_hit,
			   const Window<Wm>& win_miss)
      {
	return hit_or_miss_dispatch(mln_trait_image_kind(I)(),
				    exact(input),
				    exact(win_hit),
				    exact(win_miss));
      }

    } // end of namespace mln::morpho::internal


    template <typename I, typename Wh, typename Wm>
    inline
    mln_concrete(I)
    hit_or_miss(const Image<I>& input,
		const Window<Wh>& win_hit,
		const Window<Wm>& win_miss)
    {
      trace::entering("morpho::hit_or_miss");
      internal::hit_or_miss_tests(input, win_hit, win_miss);

      mln_concrete(I) output = internal::hit_or_miss_dispatch(input,
							      win_hit,
							      win_miss);
      trace::exiting("morpho::hit_or_miss");
      return output;
    }


    template <typename I, typename Wh, typename Wm>
    inline
    mln_concrete(I)
    hit_or_miss_opening(const Image<I>& input,
			const Window<Wh>& win_hit,
			const Window<Wm>& win_miss)
    {
      trace::entering("morpho::hit_or_miss_opening");
      internal::hit_or_miss_tests(input, win_hit, win_miss);

      mln_concrete(I) output = dilation(internal::hit_or_miss_dispatch(input,
								       win_hit,
								       win_miss),
					win::sym(win_hit));

      trace::exiting("morpho::hit_or_miss_opening");
      return output;
    }


    template <typename I, typename Wh, typename Wm>
    inline
    mln_concrete(I)
    hit_or_miss_background_opening(const Image<I>& input,
				   const Window<Wh>& win_hit,
				   const Window<Wm>& win_miss)
    {
      trace::entering("morpho::hit_or_miss_background_opening");
      internal::hit_or_miss_tests(input, win_hit, win_miss);

      mln_concrete(I) output = hit_or_miss_opening(complementation(input),
						   win_miss,
						   win_hit);

      mln_postcondition(dilation(internal::hit_or_miss_dispatch(input,
								win_hit,
								win_miss),
				 win::sym(win_miss)) == output);
      trace::exiting("morpho::hit_or_miss_background_opening");
      return output;
    }


    template <typename I, typename Wh, typename Wm>
    inline
    mln_concrete(I)
    hit_or_miss_closing(const Image<I>& input,
			const Window<Wh>& win_hit,
			const Window<Wm>& win_miss)
    {
      trace::entering("morpho::hit_or_miss_closing");
      internal::hit_or_miss_tests(input, win_hit, win_miss);

      mln_concrete(I) output =
	  complementation(hit_or_miss_opening(complementation(input),
					      win_hit, win_miss));

      // FIXME: Postcondition.
      trace::exiting("morpho::hit_or_miss_closing");
      return output;
    }


    template <typename I, typename Wh, typename Wm>
    inline
    mln_concrete(I)
    hit_or_miss_background_closing(const Image<I>& input,
				   const Window<Wh>& win_hit,
				   const Window<Wm>& win_miss)
    {
      trace::entering("morpho::hit_or_miss_background_closing");
      internal::hit_or_miss_tests(input, win_hit, win_miss);

      mln_concrete(I) output = hit_or_miss_closing(input, win_miss, win_hit);

      mln_postcondition(complementation(hit_or_miss_background_opening(
						  complementation(input),
						  win_hit, win_miss)) == output);
      trace::exiting("morpho::hit_or_miss_background_closing");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_HIT_OR_MISS_HH
