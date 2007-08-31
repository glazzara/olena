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

#ifndef MLN_MORPHO_MIN_HH
# define MLN_MORPHO_MIN_HH

# include <mln/level/compare.hh>
# include <mln/logical/and.hh>
# include <mln/arith/min.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological min: either a logical "and" (if morpho on sets)
     *  or an arithmetical min (if morpho on functions).
     */
    template <typename I, typename J, typename O>
    void min(const Image<I>& lhs, const Image<J>& rhs,
	     Image<O>& output);

    /*! Morphological min, inplace version: either a logical "and" (if
     *  morpho on sets) or an arithmetical min (if morpho on
     *  functions).
     */
    template <typename I, typename J>
    void min_inplace(Image<I>& lhs, const Image<J>& rhs);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename J, typename O>
      void min_(value::binary_kind, // binary => morphology on sets
		const Image<I>& lhs, const Image<J>& rhs,
		Image<O>& output)
      {
	return logical::and_(lhs, rhs, output);
      }

      template <typename K, typename I, typename J, typename O>
      void min_(K, // otherwise => morphology on functions
		const Image<I>& lhs, const Image<J>& rhs,
		Image<O>& output)
      {
	return arith::min(lhs, rhs, output);
      }

      // in place

      template <typename I, typename J>
      void min_inplace_(value::binary_kind, // binary => morphology on sets
			Image<I>& lhs, const Image<J>& rhs)
      {
	return logical::and_inplace(lhs, rhs);
      }

      template <typename K, typename I, typename J>
      void min_inplace_(K, // otherwise => morphology on functions
			Image<I>& lhs, const Image<J>& rhs)
      {
	return arith::min_inplace(lhs, rhs);
      }

    } // end of namespace mln::morpho::impl


    // Facades.

    template <typename I, typename J, typename O>
    void min(const Image<I>& lhs, const Image<J>& rhs, Image<O>& output)
    {
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      mln_precondition(exact(output).domain() == exact(lhs).domain());
      impl::min_(mln_value_kind(I)(), exact(lhs), exact(rhs), output);
    }

    template <typename I, typename J>
    void min_inplace(Image<I>& lhs, const Image<J>& rhs)
    {
      mln_precondition(exact(rhs).domain() == exact(lhs).domain());
      impl::min_inplace_(mln_value_kind(I)(), exact(lhs), exact(rhs));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_MIN_HH
