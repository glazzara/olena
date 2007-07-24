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

#ifndef MLN_HISTO_SORT_HH
# define MLN_HISTO_SORT_HH

/*! \file mln/histo/sort.hh
 *
 * \brief Routine to sort an histogram.
 */

# include <vector>

# include <mln/core/concept/image.hh>
# include <mln/histo/compute.hh>


namespace mln
{

  namespace histo
  {

    /// Sort points of image \p input by decreasing values.
    template <typename I>
    std::vector<mln_point(I)> sort_points_decreasing(const Image<I>& input);

    /// Sort points of image \p input by increasing values.
    template <typename I>
    std::vector<mln_point(I)> sort_points_increasing(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      std::vector<mln_point(I)>
      sort_points_decreasing(const Image<I>& input_)
      {
	const I& input = exact(input_);

	typedef mln_vset(I) S;
	const S& vset = input.values();
	const unsigned n = vset.nvalues();

	// h
	histo::data<S> h = compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
	loc[n-1] = 0;
	for (unsigned i = n - 2; i != 0; --i) 
	  loc[i] = loc[i+1] + h[i+1];

	// computing output data
	std::vector<mln_point(I)> vec(input.npoints());
	mln_fwd_piter(I) p(input.domain());
	for_all(p)
	  vec[loc[vset.index_of(input(p))]++] = p;

	return vec;
      }

      template <typename I>
      std::vector<mln_point(I)>
      sort_points_increasing(const Image<I>& input_)
      {
	const I& input = exact(input_);

	typedef mln_vset(I) S;
	const S& vset = input.values();
	const unsigned n = vset.nvalues();

	// h
	histo::data<S> h = compute(input);

	// preparing output data
	std::vector<unsigned> loc(vset.nvalues());
	loc[0] = 0;
	for (unsigned i = 1; i != n; ++i) 
	  loc[i] = loc[i-1] + h[i-1];

	// computing output data
	std::vector<mln_point(I)> vec(input.npoints());
	mln_fwd_piter(I) p(input.domain());
	for_all(p)
	  vec[loc[vset.index_of(input(p))]++] = p;

	return vec;
      }

    } // end of namespace mln::histo::impl


    template <typename I>
    std::vector<mln_point(I)>
    sort_points_decreasing(const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(exact(input).values().nvalues() > 1); // FIXME
      return impl::sort_points_decreasing(exact(input));
    }


    template <typename I>
    std::vector<mln_point(I)>
    sort_points_increasing(const Image<I>& input)
    {
      mln_precondition(exact(input).has_data());
      mln_precondition(exact(input).values().nvalues() > 1); // FIXME
      return impl::sort_points_increasing(exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::histo

} // end of namespace mln


#endif // ! MLN_HISTO_SORT_HH
