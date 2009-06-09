// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
# include <mln/util/array.hh>

/// \file
///
/// Construct from a count accumulator of a labeled image an array with
/// the ordered n biggest labels
///
/// \return an array starting at index 1


namespace mln
{

  namespace labeling
  {

    template <typename L, typename V>
    util::array<L>
    n_max(const util::array<V>& in_arr, unsigned n);


# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename V>
    util::array<L>
    n_max(const util::array<V>& in_arr, unsigned n)
    {
      trace::entering("mln::labeling::n_max");

      util::array<L> output(n + 1, 0);

      int swap = 0;
      for (unsigned i = 0; i < in_arr.nelements(); ++i)
      {
	if (in_arr[i] > in_arr[output[n]])
	{
	  output[n] = i;
	}
	int j = n - 1;
	while (j > 0 && in_arr[output[j]] < in_arr[output[j + 1]])
	{
	  swap = output[j];
	  output[j] = output[j + 1];
	  output[j + 1] = swap;
	  --j;
	}
      }

      trace::exiting("mln::labeling::n_max");
      return output;
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln

#endif // ! MLN_LABELING_N_MAX_HH
