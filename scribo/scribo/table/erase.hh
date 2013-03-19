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

#ifndef SCRIBO_TABLE_ERASE_HH
# define SCRIBO_TABLE_ERASE_HH

/// \file
///
/// \brief Erase the table lines in an image.


# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/pw/all.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <scribo/core/erase_objects.hh>

namespace scribo
{

  namespace table
  {

    using namespace mln;

    /// Erase vertical and horizontal lines from an image.
    ///
    /// \param[in]  input	  A binary image from which the table lines
    ///				  are extracted.
    /// \param[in]  hlines	  A component set with horizontal lines.
    /// \param[in]  vlines	  A component set with vertical lines.
    ///
    ///	\return A copy of \p in where the table lines are removed.
    //
    template <typename I, typename L>
    mln_concrete(I)
    erase(const Image<I>& input,
	  const component_set<L>& hlines,
	  const component_set<L>& vlines);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    inline
    mln_concrete(I)
    erase(const Image<I>& input,
	  const component_set<L>& hlines,
	  const component_set<L>& vlines)
    {
      mln_trace("scribo::internal::erase");
      mlc_equal(mln_value(I),bool)::check();
      mln_precondition(exact(input).is_valid());

      I output = duplicate(input);

      erase_objects(output, vlines);
      erase_objects(output, hlines);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::table

} // end of namespace scribo

#endif // ! SCRIBO_TABLE_ERASE_HH
