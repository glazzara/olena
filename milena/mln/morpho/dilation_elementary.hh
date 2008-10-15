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

#ifndef MLN_MORPHO_GRADIENT_ELEMENTARY_HH
# define MLN_MORPHO_GRADIENT_ELEMENTARY_HH

/// \file mln/morpho/gradient_elementary.hh
/// \brief Morphological elementary gradient.
///
/// \todo Handle border + add fastest versions.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/accu/min_max.hh>
# include <mln/level/fill.hh>


namespace mln
{

  namespace morpho
  {

    /// Morphological elementary gradient.
    ///
    /// \param[in] input The image to be dilated.
    /// \param[in] nbh The neighborhood to consider.
    ///
    /// The structuring element is the neighborhood + the center site.
    ///
    /// \{
    template <typename I, typename N>
    mln_concrete(I)
    gradient_elementary(const Image<I>& input, const Neighborhood<N>& nbh);
    /// \}


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N>
	mln_concrete(I)
	gradient_elementary_on_function(const I& input, const N& nbh)
	{
	  trace::entering("morpho::impl::generic::gradient_elementary_on_function");

	  mln_concrete(I) output;
	  initialize(output, input);

	  accu::min_max<mln_value(I)> m;
	  mln_piter(I) p(input.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    {
	      m.take_as_init(input(p));
	      for_all(n) if (input.has(n))
		m.take(input(n));
	      output(p) = m.second() - m.first();
	    }

	  trace::exiting("morpho::impl::generic::gradient_elementary_on_function");
	  return output;
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_elementary_on_set(const I& input, const N& nbh)
	{
	  trace::entering("morpho::impl::generic::gradient_elementary_on_set");

	  mln_concrete(I) output;
	  initialize(output, input);
	  level::fill(output, false);

	  mln_piter(I) p(input.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    {
	      bool b = input(p);
	      for_all(n) if (input.has(n))
		if (input(n) != b)
		  {
		    output(p) = true;
		    break;
		  }
	    }

	  trace::exiting("morpho::impl::generic::gradient_elementary_on_set");
	  return output;
	}

      } // end of namespace mln::impl::generic


      // Different cases.

      template <typename I, typename N>
      mln_concrete(I)
	gradient_elementary_(const mln::trait::image::kind::any&,
			     const mln::trait::image::speed::any&,
			     const I& input, const N& nbh)
      {
	return generic::gradient_elementary_on_function(input, nbh);
      }

      template <typename I, typename N>
      mln_concrete(I)
	gradient_elementary_(const mln::trait::image::kind::logic&,
			     const mln::trait::image::speed::any&,
			     const I& input, const N& nbh)
      {
	return generic::gradient_elementary_on_set(input, nbh);
      }


      // Selector.

      template <typename I, typename N>
      mln_concrete(I)
	gradient_elementary_(const I& input, const N& nbh)
      {
	return gradient_elementary_(mln_trait_image_kind(I)(),
				    mln_trait_image_speed(I)(),
				    input, nbh);
      }

    } // end of namespace mln::impl


    // Facade.

    template <typename I, typename N>
    mln_concrete(I)
    gradient_elementary(const Image<I>& input_, const Neighborhood<N>& nbh_)
    {
      const I& input = exact(input_);
      const N& nbh   = exact(nbh_);

      trace::entering("morpho::gradient_elementary");
      mln_precondition(input.has_data());
      // FIXME: mln_precondition(! nbh.is_empty());

      mln_concrete(I) output = impl::gradient_elementary_(input, nbh);

      trace::exiting("morpho::gradient_elementary");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_GRADIENT_ELEMENTARY_HH
