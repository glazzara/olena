// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LEVEL_SORT_PSITES_HH
# define MLN_LEVEL_SORT_PSITES_HH

/// \file mln/level/sort_psites.hh
/// \brief Sort_Psites the contents of an image into another one.
///
/// \todo Factor code + optimize.

# include <algorithm>

# include <mln/core/concept/image.hh>
# include <mln/convert/to_p_array.hh>
# include <mln/histo/compute.hh>


namespace mln
{

  namespace level
  {

    /*! Sort psites the image \p input through a function \p f to set
     *  the \p output image in increasing way.
     *
     * \param[in] input The input image.
     *
     * \pre \p input.has_data
     */
    template <typename I>
    p_array<mln_psite(I)> sort_psites_increasing(const Image<I>& input);

    /*! Sort psites the image \p input through a function \p f to set
     *  the \p output image in decreasing way.
     *
     * \param[in] input The input image.
     *
     * \pre \p input.has_data
     */
    template <typename I>
    p_array<mln_psite(I)> sort_psites_decreasing(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // utility

      template <typename I>
      struct value_psite_less_
      {
	const I& ima_;

	inline
	value_psite_less_(const I& ima)
	  : ima_(ima)
	{
	}

	inline
	bool operator()(const mln_psite(I)& lhs,
			const mln_psite(I)& rhs) const
	{
	  return ima_(lhs) < ima_(rhs) || (ima_(lhs) == ima_(rhs)
					   && lhs < rhs);
	}
      };

      template <typename I>
      struct value_psite_greater_
      {
	const I& ima_;

	inline
	value_psite_greater_(const I& ima)
	  : ima_(ima)
	{
	}

	inline
	bool operator()(const mln_psite(I)& lhs,
			const mln_psite(I)& rhs) const
	{
	  return ima_(lhs) > ima_(rhs) || (ima_(lhs) == ima_(rhs)
					   && lhs > rhs);
	}
      };


      // increasing

      template <typename I>
      inline
      p_array<mln_psite(I)>
      sort_psites_increasing_(trait::image::quant::any, // general case
			      const I& input)
      {
	p_array<mln_psite(I)> v = convert::to_p_array(input.domain());
	std::sort(v.hook_().begin(), v.hook_().end(),
		  value_psite_less_<I>(input));
	return v;
      }

      template <typename I>
      inline
      p_array<mln_psite(I)>
      sort_psites_increasing_(trait::image::quant::low, // low quantization
			      const I& input)
      {
	typedef mln_vset(I) S;
	const S& vset = input.values_eligible();
	const unsigned n = vset.nvalues();

	// h
	histo::data<mln_value(I)> h = histo::compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
	loc[0] = 0;
	for (unsigned i = 1; i != n; ++i)
	  loc[i] = loc[i-1] + h[i-1];

	// computing output data
	std::vector<mln_psite(I)> vec(input.nsites());
	mln_fwd_piter(I) p(input.domain());
	for_all(p)
	  vec[loc[vset.index_of(input(p))]++] = p;

 	p_array<mln_psite(I)> v(vec);
	return v;
      }


      // decreasing

      template <typename I>
      inline
      p_array<mln_psite(I)>
      sort_psites_decreasing_(trait::image::quant::any, // general case
			      const I& input)
      {
	p_array<mln_psite(I)> v = convert::to_p_array(input.domain());
	std::sort(v.hook_().begin(), v.hook_().end(),
		  value_psite_greater_<I>(input));
	return v;
      }

      template <typename I>
      inline
      p_array<mln_psite(I)>
      sort_psites_decreasing_(trait::image::quant::low, // low quantization
			      const I& input)
      {
	typedef mln_vset(I) S;
	const S& vset = input.values_eligible();
	const unsigned n = vset.nvalues();

	// h
	histo::data<mln_value(I)> h = histo::compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
	loc[n-1] = 0;
	for (int i = n - 2; i >= 0; --i)
	  loc[i] = loc[i+1] + h[i+1];

	// computing output data
	std::vector<mln_psite(I)> vec(input.nsites());
	mln_fwd_piter(I) p(input.domain());
	for_all(p)
	  vec[loc[vset.index_of(input(p))]++] = p;

 	p_array<mln_psite(I)> v(vec);
	return v;
      }


    } // end of namespace mln::level::impl


    // Facades.

    template <typename I>
    inline
    p_array<mln_psite(I)>
    sort_psites_increasing(const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      return impl::sort_psites_increasing_(mln_trait_image_quant(I)(),
					   exact(input));
    }

    template <typename I>
    inline
    p_array<mln_psite(I)>
    sort_psites_decreasing(const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      return impl::sort_psites_decreasing_(mln_trait_image_quant(I)(),
					   exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_SORT_PSITES_HH
