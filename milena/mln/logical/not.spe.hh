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

#ifndef MLN_LOGICAL_NOT_SPE_HH
# define MLN_LOGICAL_NOT_SPE_HH

/*! \file
 *
 * \brief Specializations for mln::logical::not.
 *
 */

# ifndef MLN_LOGICAL_NOT_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LOGICAL_NOT_HH

# include <mln/core/concept/image.hh>


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace logical
  {

    namespace impl
    {

      namespace generic
      {
	template <typename I, typename O>
	void not_(const I& input, O& output);

	template <typename I>
	void not_inplace(I& inout);
      }

      template <typename I, typename O>
      inline
      void not_(trait::image::speed::any, const I& input, O& output)
      {
	generic::not_(input, output);
      }

      template <typename I>
      inline
      void not_inplace(trait::image::speed::any, I& inout)
      {
	generic::not_inplace(inout);
      }

      template <typename I, typename O>
      inline
      void not_(trait::image::speed::fastest, const I& input, O& output)
      {
	mln_trace("logical::impl::not_");

	mln_pixter(const I) ip(input);
	mln_pixter(O)       op(output);
	for_all_2(ip, op)
	  op.val() = ! ip.val();

      }

      template <typename I>
      inline
      void not_inplace(trait::image::speed::fastest, I& inout)
      {
	mln_trace("logical::impl::not_inplace");

	mln_pixter(I) p(inout);
	for_all(p)
	  p.val() = ! p.val();

      }

    } // end of namespace mln::logical::impl

  } // end of namespace mln::logical

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LOGICAL_NOT_SPE_HH
