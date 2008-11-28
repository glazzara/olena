// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_INTERNAL_ELEMENTARY_HH
# define MLN_MORPHO_INTERNAL_ELEMENTARY_HH

/// \file mln/morpho/internal/elementary.hh
/// \brief Morphological elementary operator.
///
/// \todo Add fastest version.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/meta_accumulator.hh>


namespace mln
{

  namespace morpho
  {

    namespace internal
    {

      /// Morphological elementary operator.
      template <typename A, typename I, typename N, typename O>
      mln_concrete(I)
	elementary(const I& input, const N& nbh, O oper);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Different cases.

	template <typename I, typename N, typename A, typename O>
	mln_concrete(I)
	  elementary_(mln::trait::image::speed::any,
		      const I& input, const N& nbh, A accu, O oper)
	{
	  mln_concrete(I) output;
	  initialize(output, input);

	  mln_piter(I) p(input.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    {
	      accu.take_as_init(input(p));
	      for_all(n) if (input.has(n))
		accu.take(input(n));
	      output(p) = oper(accu, input(p));
	    }

	  return output;
	}

	// Selector.

	template <typename A, typename I, typename N, typename O>
	mln_concrete(I)
 	elementary_selector_(const I& input, const N& nbh, O oper)
	{
 	  mln_accu_with(A, mln_value(I)) accu;
	  return elementary_(mln_trait_image_speed(I)(),
 			     input, nbh,
 			     accu, oper);
	}

      } // end of namespace mln::morpho::internal::impl


      // Facade.

      template <typename A, typename I, typename N, typename O>
      mln_concrete(I)
	elementary(const I& input, const N& nbh, O oper)
      {
	return impl::elementary_selector_<A>(exact(input), exact(nbh), oper);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::internal

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_INTERNAL_ELEMENTARY_HH
