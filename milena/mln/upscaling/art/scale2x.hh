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

#ifndef MLN_UPSCALING_ART_SCALE2X_HH
# define MLN_UPSCALING_ART_SCALE2X_HH

/// \file
///
/// 2X upscaling algorithm for pixel art images.


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

      /*! \brief 2X upscaling algorithm for pixel art images.

	\param[in] input An image.

	\return An upscaled image.


	Source:
	http://en.wikipedia.org/wiki/Pixel_art_scaling_algorithms

	\ingroup mlngeom
      */
      template <typename I>
      mln_concrete(I)
      scale2x(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      mln_concrete(I)
      scale2x(const Image<I>& input_)
      {
	trace::entering("mln::upscaling::art::scale2x");

	const I& input = exact(input_);
	mln_precondition(input.is_valid());
	mlc_is_a(mln_domain(I), Box)::check();

	extension::adjust_duplicate(input, 1);

	mln_domain(I) ext_domain(input.domain().pmin() * 2,
				 input.domain().pmax() * 2
				 + mln::down_right);

	mln_concrete(I) output(ext_domain);

	mln_piter(I) p(input.domain());
	for_all(p)
	{

	  //   A    --\ 1 2
	  // C P B  --/ 3 4
	  //   D
	  mln_site(I)
	    pA = p + mln::up,
	    pB = p + mln::right,
	    pC = p + mln::left,
	    pD = p + mln::down,
	    pOut = p * 2;

	  //  IF C==A AND C!=D AND A!=B => 1=A
	  if (input(pC) == input(pA)
	      && input(pC) != input(pD)
	      && input(pA) != input(pB))
	    output(pOut) = input(pA);
	  else
	    output(pOut) = input(p);

	  //  IF A==B AND A!=C AND B!=D => 2=B
	  if (input(pA) == input(pB)
	      && input(pA) != input(pC)
	      && input(pB) != input(pD))
	    output(pOut + mln::right) = input(pB);
	  else
	    output(pOut + mln::right) = input(p);

	  //  IF D==C AND D!=B AND C!=A => 3=C
	  if (input(pD) == input(pC)
	      && input(pD) != input(pB)
	      && input(pC) != input(pA))
	    output(pOut + mln::down) = input(pC);
	  else
	    output(pOut + mln::down) = input(p);

	  //  IF B==D AND B!=A AND D!=C => 4=D
	  if (input(pB) == input(pD)
	      && input(pB) != input(pA)
	      && input(pD) != input(pC))
	    output(pOut + mln::down_right) = input(pD);
	  else
	    output(pOut + mln::down_right) = input(p);

	  }

	trace::exiting("mln::upscaling::art::scale2x");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::upscaling::art

  } // end of namespace mln::upscaling

} // end of namespace mln


#endif // ! MLN_UPSCALING_ART_SCALE2X_HH
