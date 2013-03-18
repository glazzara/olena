// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ELEMENTARY_TOP_HAT_HH
# define MLN_MORPHO_ELEMENTARY_TOP_HAT_HH

/// \file
///
/// Morphological elementary top-hats.
///
/// \todo Save memory.

# include <mln/morpho/elementary/opening.hh>
# include <mln/morpho/elementary/closing.hh>
# include <mln/morpho/minus.hh>


namespace mln
{

  namespace morpho
  {

    namespace elementary
    {

      /// Morphological elementary white top-hat (for object / light objects).
      ///
      /// This operator is Id - ope.
      //
      template <typename I, typename N>
      mln_concrete(I)
      top_hat_white(const Image<I>& input, const Neighborhood<N>& nbh);


      /// Morphological elementary black top-hat (for background / dark objects).
      ///
      /// This operator is clo - Id.
      //
      template <typename I, typename N>
      mln_concrete(I)
      top_hat_black(const Image<I>& input, const Neighborhood<N>& nbh);


      /// Morphological elementary self-complementary top-hat.
      ///
      /// This operator is \n
      ///   = top_hat_white + top_hat_black \n
      ///   = (Id - opening) + (closing - Id) \n
      ///   = closing - opening. \n
      //
      template <typename I, typename N>
      mln_concrete(I)
      top_hat_self_complementary(const Image<I>& input, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename N>
      inline
      mln_concrete(I)
      top_hat_white(const Image<I>& input, const Neighborhood<N>& nbh)
      {
	mln_trace("morpho::elementary::top_hat_white");

	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	mln_concrete(I) output = morpho::minus(input,
					       elementary::opening(input, nbh));

	mln_postcondition(test::positive(output));

	return output;
      }


      template <typename I, typename N>
      inline
      mln_concrete(I)
      top_hat_black(const Image<I>& input, const Neighborhood<N>& nbh)
      {
	mln_trace("morpho::elementary::top_hat_black");

	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	mln_concrete(I) output = morpho::minus(elementary::closing(input, nbh),
					       input);

	mln_postcondition(test::positive(output));

	return output;
      }


      template <typename I, typename N>
      inline
      mln_concrete(I)
      top_hat_self_complementary(const Image<I>& input, const Neighborhood<N>& nbh)
      {
	mln_trace("morpho::elementary::top_hat_self_complementary");
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	mln_concrete(I) output = morpho::minus(elementary::closing(input, nbh),
					       elementary::opening(input, nbh));

	mln_postcondition(test::positive(output));
	// mln_postcondition(output == white + black);

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::elementary

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ELEMENTARY_TOP_HAT_HH
