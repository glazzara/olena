// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_ARITH_DIFF_ABS_HH
# define MLN_ARITH_DIFF_ABS_HH

/// \file
///
/// Point-wise addition between images.
///
/// \todo Speedup; some versions are not optimal.

# include <mln/arith/includes.hh>
# include <mln/fun/vv2v/diff_abs.hh>
# include <mln/data/transform.hh>

namespace mln
{

  namespace arith
  {

    /// Point-wise absolute difference of images \p lhs and \p rhs.
    /*!
     * \param[in] lhs First operand image.
     * \param[in] rhs Second operand image.
     * \result The result image.
     *
     * \pre \p lhs.domain == \p rhs.domain
     */
    template <typename I>
    mln_concrete(I)
    diff_abs(const Image<I>& lhs, const Image<I>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_concrete(I)
    diff_abs(const Image<I>& lhs, const Image<I>& rhs)
    {
      mln_trace("arith::diff_abs");
      mln_precondition(exact(lhs).is_valid());
      mln_precondition(exact(rhs).is_valid());
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());

      mln_concrete(I) output = data::transform(lhs, rhs, fun::vv2v::diff_abs<mln_value(I)>());

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_DIFF_ABS_HH
