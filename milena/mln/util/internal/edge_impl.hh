// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_EDGE_IMPL_HH
# define MLN_CORE_INTERNAL_EDGE_IMPL_HH

/// \file mln/util/internal/edge_impl.hh
///
/// Define a couple of implementation classes to provide methods
/// to classes of generalized edges.

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace util
  {

    namespace internal
    {

      /// Implementation class to equip generalized edge classes.
      template <typename G>
      class edge_impl_
      {
	protected:
	  edge_impl_();
      };

    } // end of namespace internal

  } // end of namespace util

} // end of namespace mln

#ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace util
  {

    namespace internal
    {

    template <typename G>
    inline
    edge_impl_<G>::edge_impl_()
    {
    }

    } // end of namespace internal

  } // end of namespace util

} // end of namespace mln

#endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_CORE_INTERNAL_EDGE_IMPL_HH
