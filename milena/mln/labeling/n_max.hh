// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
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

#ifndef MLN_LABELING_N_MAX_HH
# define MLN_LABELING_N_MAX_HH

# include <mln/core/concept/image.hh>
# include <mln/accu/math/count.hh>
# include <mln/labeling/compute.hh>
# include <mln/util/array.hh>

/// \file
///
/// Construct from a count accumulator of a labeled image an array with
/// the ordered n biggest labels.

namespace mln
{

  namespace labeling
  {

    /*! \brief Construct from a count accumulator of a labeled image an array with
      the ordered n biggest labels.

      \return an array starting at index 1

      \warning The biggest value is stored at index 0. It usually
      represents the background and should be ignored. However, it
      is still accessible if necessary.

      \ingroup labeling
    */
    template <typename L>
    util::array<mln_value(L)>
    n_max(const Image<L>& lbl, const mln_value(L)& nlabels, unsigned n);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename L, typename V>
      util::array<L>
      n_max(const util::array<V>& in_arr, unsigned n)
      {
	mln_trace("mln::labeling::n_max");

	mln_precondition(n < in_arr.nelements());

	util::array<L> output(n + 1, 0);
	for (unsigned i = 0; i < n + 1; ++i)
	  output[i] = i;
	int swap = 0;
	for (int j = n - 1; j > 0; --j)
	{
	  if (in_arr[output[j]] < in_arr[output[j + 1]])
	  {
	    swap = output[j];
	    output[j] = output[j + 1];
	    output[j + 1] = swap;
	  }
	}

	for (unsigned i = n; i < in_arr.nelements(); ++i)
	{
	  if (in_arr[i] > in_arr[output[n]])
	  {
	    output[n] = i;
	    for (unsigned j = n - 1; j > 0 && in_arr[output[j]] < in_arr[output[j + 1]]; --j)
	    {
	      swap = output[j];
	      output[j] = output[j + 1];
	      output[j + 1] = swap;
	    }
	  }
	}

	return output;
      }

    } // end of namespace mln::labeling::internal


    template <typename L>
    util::array<mln_value(L)>
    n_max(const Image<L>& lbl, const mln_value(L)& nlabels, unsigned n)
    {
      mln_precondition(exact(lbl).is_valid());

      typedef accu::math::count<mln_site(L)> accu_t;
      accu_t accu;

      util::array<mln_result(accu_t)>
	counts = labeling::compute(accu, lbl, nlabels);

      return n_max<mln_value(L)>(counts, n);
    }


# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln

#endif // ! MLN_LABELING_N_MAX_HH
