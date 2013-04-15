// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_UPSCALING_ART_SCALE3X_HH
# define MLN_UPSCALING_ART_SCALE3X_HH

/// \file
///
/// 3X upscaling algorithm for pixel art images.


# include <mln/core/concept/box.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/alias/dpoint2d.hh>

# include <mln/extension/adjust_duplicate.hh>


namespace mln
{

  namespace upscaling
  {

    namespace art
    {

      /*! \brief 3X upscaling algorithm for pixel art images.

	\param[in] input An image.

	\return An upscaled image.


	Source:
	http://en.wikipedia.org/wiki/Pixel_art_scaling_algorithms

	\ingroup mlngeom
      */
      template <typename I>
      mln_concrete(I)
      scale3x(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      scale3x(const Image<I>& input_)
      {
	mln_trace("mln::upscaling::art::scale3x");

	const I& input = exact(input_);
	mln_precondition(input.is_valid());
	mlc_is_a(mln_domain(I), Box)::check();

	extension::adjust_duplicate(input, 1);

 	mln_domain(I) ext_domain(input.domain().pmin() * 3,
				 input.domain().pmax() * 3
				 + 2 * mln::down_right);

	mln_concrete(I) output(ext_domain);


	mln_piter(I) p(input.domain());
	for_all(p)
	{

	  // A B C --\  1 2 3
	  // D E F    > 4 5 6
	  // G H I --/  7 8 9
	  mln_site(I)
	    pA = p + mln::up_left,
	    pB = p + mln::up,
	    pC = p + mln::up_right,
	    pD = p + mln::left,
	    pE = p,
	    pF = p + mln::right,
	    pG = p + mln::down_left,
	    pH = p + mln::down,
	    pI = p + mln::down_right,
	    pOut = p * 3 + mln::down_right;

	  //  IF D==B AND D!=H AND B!=F => 1=D
	  if (input(pD) == input(pB)
	      && input(pD) != input(pH)
	      && input(pB) != input(pF))
	    output(pOut + mln::up_left) = input(pD);
	  else
	    output(pOut + mln::up_left) = input(p);

	  //  IF (D==B AND D!=H AND B!=F AND E!=C) OR (B==F AND B!=D
	  //  AND F!=H AND E!=A) 2=B
	  if ((input(pD) == input(pB)
	       && input(pD) != input(pH)
	       && input(pB) != input(pF)
	       && input(pE) != input(pC))
	      || (input(pB) == input(pF)
		  && input(pB) != input(pD)
		  && input(pF) != input(pH)
		  && input(pE) != input(pA)))
	    output(pOut + mln::up) = input(pB);
	  else
	    output(pOut + mln::up) = input(p);

	  //  IF B==F AND B!=D AND F!=H => 3=F
	  if (input(pB) == input(pF)
	      && input(pB) != input(pD)
	      && input(pF) != input(pH))
	    output(pOut + mln::up_right) = input(pF);
	  else
	    output(pOut + mln::up_right) = input(p);

	  //  IF (H==D AND H!=F AND D!=B AND E!=A) OR (D==B AND D!=H
	  //  AND B!=F AND E!=G) 4=D
	  if ((input(pH) == input(pD)
	       && input(pH) != input(pF)
	       && input(pD) != input(pB)
	       && input(pE) != input(pA))
	      || (input(pD) == input(pB)
		  && input(pD) != input(pH)
		  && input(pB) != input(pF)
		  && input(pE) != input(pG)))
	    output(pOut + mln::left) = input(pD);
	  else
	    output(pOut + mln::left) = input(p);

	  //  5=E
	  output(pOut) = input(p);

	  //  IF (B==F AND B!=D AND F!=H AND E!=I) OR (F==H AND F!=B
	  //  AND H!=D AND E!=C) 6=F
	  if ((input(pB) == input(pF)
	       && input(pB) != input(pD)
	       && input(pF) != input(pH)
	       && input(pE) != input(pI))
	      || (input(pF) == input(pH)
		  && input(pF) != input(pB)
		  && input(pH) != input(pD)
		  && input(pE) != input(pC)))
	    output(pOut + mln::right) = input(pF);
	  else
	    output(pOut + mln::right) = input(p);

	  //  IF H==D AND H!=F AND D!=B => 7=D
	  if (input(pH) == input(pD)
	      && input(pH) != input(pF)
	      && input(pD) != input(pB))
	    output(pOut + mln::down_left) = input(pD);
	  else
	    output(pOut + mln::down_left) = input(p);


	  //  IF (F==H AND F!=B AND H!=D AND E!=G) OR (H==D AND H!=F
	  //  AND D!=B AND E!=I) 8=H
	  if ((input(pF) == input(pH)
	       && input(pF) != input(pB)
	       && input(pH) != input(pD)
	       && input(pE) != input(pG))
	      || (input(pH) == input(pD)
		  && input(pH) != input(pF)
		  && input(pD) != input(pB)
		  && input(pE) != input(pI)))
	    output(pOut + mln::down) = input(pH);
	  else
	    output(pOut + mln::down) = input(p);


	  //  IF F==H AND F!=B AND H!=D => 9=F
	  if (input(pF) == input(pH)
	      && input(pF) != input(pB)
	      && input(pH) != input(pD))
	    output(pOut + mln::down_right) = input(pF);
	  else
	    output(pOut + mln::down_right) = input(p);

	}

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::upscaling::art

  } // end of namespace mln::upscaling

} // end of namespace mln


#endif // ! MLN_UPSCALING_ART_SCALE3X_HH
