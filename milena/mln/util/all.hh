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

#ifndef MLN_UTIL_ALL_HH
# define MLN_UTIL_ALL_HH

/// \file mln/util/all.hh
///
/// File that includes all util-related routines.


namespace mln
{

  /// Namespace of tools using for more complex algorithm.
  namespace util {

    /// Implementation namespace of util namespace.
    namespace impl {}

  }

}

# include <mln/util/array.hh>
# include <mln/util/branch_iter.hh>
# include <mln/util/branch_iter_ind.hh>
# include <mln/util/couple.hh>
# include <mln/util/dindex.hh>
# include <mln/util/eat.hh>
# include <mln/util/edge.hh>
# include <mln/util/graph.hh>
# include <mln/util/greater_point.hh>
# include <mln/util/greater_psite.hh>
# include <mln/util/ignore.hh>
# include <mln/util/index.hh>
# include <mln/util/lazy_set.hh>
# include <mln/util/lemmings.hh>
# include <mln/util/multi_site.hh>
# include <mln/util/nil.hh>
# include <mln/util/ord.hh>
# include <mln/util/ord_pair.hh>
# include <mln/util/pix.hh>
# include <mln/util/set.hh>
# include <mln/util/site_pair.hh>
//# include <mln/util/timer.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/util/tree.hh>
# include <mln/util/tree_fast.hh>
# include <mln/util/tree_to_fast.hh>
# include <mln/util/vertex.hh>
# include <mln/util/yes.hh>

#endif // ! MLN_UTIL_ALL_HH
