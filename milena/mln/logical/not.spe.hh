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

#ifndef MLN_LOGICAL_NOT_SPE_HH
# define MLN_LOGICAL_NOT_SPE_HH

/*! \file mln/logical/not.spe.hh
 *
 * \brief Specializations for mln::logical::not.
 *
 */

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
	void not__(const I& input, O& output);
      }

      template <typename I, typename O>
      inline
      void not__(trait::image::speed::any, const I& input, O& output)
      {
	generic::not__(input, output);
      }

      template <typename I, typename O>
      inline
      void not__(trait::image::speed::fastest, const I& input, O& output)
      {
	trace::entering("logical::impl::not__");

	mln_pixter(const I) ip(input);
	mln_pixter(O)       op(output);
	for_all_2(ip, op)
	  op.val() = ! ip.val();

	trace::exiting("logical::impl::not__");
      }

    } // end of namespace mln::logical::impl

  } // end of namespace mln::logical

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LOGICAL_NOT_HH
