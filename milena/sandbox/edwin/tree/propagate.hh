// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_PROPAGATE_HH_
# define MLN_MORPHO_TREE_PROPAGATE_HH_

/// \file mln/morpho/tree/propagate.hh
///
/// Functions to propagate a node value in the tree.

#include <mln/morpho/tree/data.hh>

namespace mln {
  namespace morpho {
    namespace tree {


      /// Propagate the representative point's value to
      /// non-representative points of the same node.
      template <typename T, typename A>
      void
      propagate_representant(const T& t, Image<A>& a_)
      {
	A a = exact(a_);
	mln_up_site_piter(T) p(t);
	for_all(p)
	  if (! t.is_a_node(p))
	    {
	      mln_assertion(t.is_a_node(t.parent(p)));
	      a(p) = a(t.parent(p));
	    }
      }

      /**
      ** For each component in the list \p component_list, it
      ** propagates the representant value to the remaining nodes of the
      ** component. The value of a node that doesn't belong to a component is
      ** set to \p null.
      **
      ** @param attr_image The attribute image.
      ** @param tree The component tree used to propagate value.
      ** @param component_list The list of components.
      ** @param null The nodes that don't belong to components will be set
      ** with this value.
      **
      ** @return The resulting component image.
      */
      template <typename A, typename T>
      inline
      A set_value_to_components(const Image<A>& attr_image,
				const T& tree,
				const p_array< mln_psite(A) >& component_list,
				const mln_value(A)& null)
      {
	const A& attr_img = exact(attr_image);
	A out;
	initialize(out, attr_img);
	data::fill(out, null);

	mln_piter(p_array<mln_psite(A)>) p(component_list);
	for_all(p)
	{
	  out(p) = attr_img(p);
	  morpho::tree::propagate_node_to_descendants(p, tree, out);
	}
	morpho::tree::propagate_representant(tree, out);
	return out;
      }

    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_HH_ */


