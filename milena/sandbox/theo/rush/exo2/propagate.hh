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

#ifndef MLN_MORPHO_TREE_PROPAGATE_HH_
# define MLN_MORPHO_TREE_PROPAGATE_HH_

/// \file mln/morpho/tree/propagate.hh
///
/// Functions to propagate a node value in the tree.

#include <mln/morpho/tree/data.hh>

namespace mln {
  namespace morpho {
    namespace tree {


      /// The representative point propagates its value to
      /// its non-representative children nodes.
      template <typename T, typename A>
      void
      back_propagate_level(const T& t, A& a)
      {
	mln_fwd_piter(T) p(t.domain());
	for_all(p)
	  if (! t.is_a_node(p))
	    {
	      mln_assertion(t.is_a_node(t.parent(p)));
	      a(p) = a(t.parent(p));
	    }
      }

      /// The representative point having the right
      /// value propagates to the nodes of its sub-branch.
      template <typename T, typename A>
      void
      back_propagate_subbranch(const T& t, A& a, mln_value(A) v)
      {
	mln_bkd_piter(T) p(t.domain());
	for_all(p)
	  if (t.is_a_node(p) && a(t.parent(p)) == v)
	    {
	      mln_assertion(t.is_a_node(t.parent(p)));
	      a(p) = a(t.parent(p));
	    }
      }
    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_HH_ */


