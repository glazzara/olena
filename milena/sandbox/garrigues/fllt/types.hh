// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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


#ifndef MLN_FIXME_FLLT_TYPES_HH
# define MLN_FIXME_FLLT_TYPES_HH

/*! \file types.hh
 *
 * \brief Types for Fast level line transform.
 *
 */

# include <mln/core/p_set.hh>
# include <mln/util/tree.hh>
# include <mln/util/branch_iter_ind.hh>

namespace mln
{
  namespace fllt
  {

    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      p_set<P> points;
      p_set<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
    };

# define fllt_tree(P, V)  util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  util::tree_node< fllt_node_elt<P, V> >
# define fllt_branch(P, V)  util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter_ind(P, V)  util::branch_iter_ind< fllt_node_elt<P, V> >

    //    # define fllt_node(P, V)  typename fllt_tree(P, V)::node_t

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_TYPES_HH
