// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_CLOSING_SUM_HH
# define MLN_MORPHO_CLOSING_SUM_HH

/// \file mln/morpho/closing/sum.hh
///
/// Morphological sum closing.

# include <mln/morpho/closing/leveling.hh>
# include <mln/morpho/attribute/sum.hh>


namespace mln
{

  namespace morpho
  {

    namespace closing
    {

      /// Morphological sum closing.
      template <typename I, typename N>
      mln_concrete(I)
      sum(const Image<I>& input, const Neighborhood<N>& nbh,
	  unsigned lambda);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N>
      inline
      mln_concrete(I)
      sum(const Image<I>& input, const Neighborhood<N>& nbh,
	  unsigned lambda)
      {
	trace::entering("morpho::closing::sum");

	mln_precondition(exact(input).is_valid());

	mln_concrete(I) output;
	output = closing::leveling(input, nbh, attribute::sum<I, float>(), lambda);

	trace::exiting("morpho::closing::sum");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::closing

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CLOSING_SUM_HH
