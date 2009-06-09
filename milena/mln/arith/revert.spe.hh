// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

      namespace generic
      {
	template <typename I, typename O>
	void revert_(const I& input, O& output);
      }

      template <typename I, typename O>
      inline
      void revert_(trait::image::speed::any, const I& input, O& output)
      {
	generic::revert_(input, output);
      }

      template <typename I, typename O>
      inline
      void revert_(trait::image::speed::fastest, const I& input, O& output)
      {
	trace::entering("arith::impl::revert_");

	typedef mln_value(I) V;
	mln_pixter(const I) ip(input);
	mln_pixter(O)       op(output);
	for_all_2(ip, op)
	  op.val() = mln_min(V) + (mln_max(V) - ip.val());

	trace::entering("arith::impl::revert_");
      }

    } // end of namespace mln::arith::impl

  } // end of namespace mln::arith

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_ARITH_REVERT_SPE_HH
