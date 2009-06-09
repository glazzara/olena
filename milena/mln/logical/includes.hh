// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LOGICAL_INCLUDES_HH
# define MLN_LOGICAL_INCLUDES_HH

/// \file


# include <mln/core/concept/image.hh>
# include <mln/data/transform.hh>
# include <mln/data/transform_inplace.hh>
# include <mln/fun/vv2v/macros.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace logical
  {

    namespace internal
    {

      template <typename L, typename R>
      inline
      void
      tests(const Image<L>& lhs_, const Image<R>& rhs_)
      {
	const L& lhs = exact(lhs_);
	const R& rhs = exact(rhs_);

	mln_precondition(lhs.is_valid());
	mln_precondition(rhs.is_valid());
	mln_precondition(rhs.domain() == lhs.domain());

	(void) lhs;
	(void) rhs;
      }
      
    } // end of namespace mln::logical::internal

  } // end of namespace mln::logical

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LOGICAL_INCLUDES_HH
