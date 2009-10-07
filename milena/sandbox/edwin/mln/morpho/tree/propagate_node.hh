// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_PROPAGATE_NODE_HH
# define MLN_MORPHO_TREE_PROPAGATE_NODE_HH

/// \file
///
/// Functions to propagate node in the tree.
///
/// \todo: The new implementation permits: + to propagate toward
/// subcomponents very fast (data::fill)\ + to propagate toward
/// supernodes as fast as the previous component\ + the attribute
/// image is still valid without the use of propagate_representative
/// that does not make sense any more.
///
/// We are able in both cases to do something like:
/// + data::fill(attr_img | asc(n), v)
/// + data::fill(attr_img | desc(n), v)
///
/// However the subimage morpher is not specialized for fast image and
/// does not take benefits in the fact that:
/// + desc(n) is a p_run (so can be memseté)
/// + attr_img can accessed by indexed (element() method for fast-image)

# include <mln/core/image/attribute_image.hh>
# include <algorithm>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename T, typename V>
      inline
      unsigned
      propagate_node_to_descendants(attribute_image<T, V>& a,
				    const typename T::node_t& n,
				    const V& value)
      {
	typedef typename T::nodes_t desc_t;
	mln_precondition(a.is_valid());
	mln_precondition(a.has(n));

	// (n € desc(n)) but we don't set its value.
	unsigned i = a.tree().desc(n).nsites();

	if (i > 0)
	  {
	    mln_assertion(a.tree().has_index(n.index() + i));
	    V* v = a.buffer() + n.index();
	    std::fill(v + 1, v + i, value);
	  }
	return i - 1;
      }

      template <typename T, typename V>
      inline
      unsigned
      propagate_node_to_ancestors(attribute_image<T, V>& a,
				  typename T::node_t n,
				  const V& v)
      {
	mln_precondition(a.is_valid());
	mln_precondition(a.has(n));

	if (a.tree().is_a_root(n))
	  return 0;

	unsigned i = 0;
	do {
	  n = a.tree().parent(n);
	  a(n) = v;
	  ++i;
	} while (!a.tree().is_a_root(n));

	return i;
      }

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_PROPAGATE_NODE_HH
