// Copyright (C) 2007, 2008, 2009, 2010, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_ARITH_REVERT_SPE_HH
# define MLN_ARITH_REVERT_SPE_HH

/// \file
///
/// Specializations for mln::arith::revert.

# ifndef MLN_ARITH_REVERT_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_ARITH_REVERT_HH

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace arith
  {

    namespace impl
    {

      // FIXME: This is a fast implementation not a fastest one!
      template <typename I, typename O>
      inline
      void revert_fastest(const Image<I>& input_, Image<O>& output_)
      {
	mln_trace("arith::impl::revert_fastest");

	const I& input = exact(input_);
	O& output = exact(output_);

	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());
	mln_precondition(input.domain() == output.domain());

	typedef mln_value(I) V;
	mln_pixter(const I) ip(input);
	mln_pixter(O)       op(output);
	for_all_2(ip, op)
	  op.val() = mln_min(V) + (mln_max(V) - ip.val());
      }

    } // end of namespace mln::arith::impl

  } // end of namespace mln::arith

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_ARITH_REVERT_SPE_HH
