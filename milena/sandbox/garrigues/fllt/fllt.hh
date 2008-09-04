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


#ifndef MLN_FIXME_FLLT_HH
# define MLN_FIXME_FLLT_HH

/*! \file fllt.hh
 *
 * \brief Fast level line transform of an image.
 *
 */

# include <mln/core/image/image2d.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/core/image/inplace.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/pset_if_piter.hh>
# include <mln/core/site_set/pset_if.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/routine/clone.hh>
# include <mln/core/a_point_of.hh>

# include <mln/debug/println.hh>
# include <mln/debug/println_with_border.hh>

# include <mln/border/fill.hh>

# include <mln/level/compute.hh>
# include <mln/level/fill.hh>
# include <mln/accu/min.hh>
# include <mln/accu/max.hh>

# include <mln/set/uni.hh>
# include <mln/set/diff.hh>
# include <mln/set/inter.hh>
# include <mln/set/is_subset_of.hh>

# include <mln/util/tree.hh>
# include <mln/util/branch_iter_ind.hh>

# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/regional_maxima.hh>
# include <mln/labeling/level.hh>

# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>

# include <mln/util/tree_to_image.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/stretch.hh>
# include <mln/level/compare.hh>
# include <mln/io/pgm/save.hh>

# include "types.hh"
# include "debug.hh"

# include "lower.hh"
# include "upper.hh"

# include "compute_level_set.hh"
# include "merge.hh"

namespace mln
{
  namespace fllt
  {

    template <typename V>
    // Fixme : return type
    fllt_tree(point2d, V)
    fllt(const image2d<V>& ima)
    {
      typedef point2d P;

      fllt_tree(P, V) upper_tree;
      fllt_tree(P, V) lower_tree;
      image2d<fllt_node(P, V)*> low_reg(ima.domain());
      image2d<fllt_node(P, V)*> upp_reg(ima.domain());

      std::cout << "1/ Compute the lower level set." << std::endl;
      lower_tree = compute_level_set<V, lower<V> >(ima, low_reg);
      //draw_tree(ima, lower_tree);
      std::cout << "2/ Compute the upper level set." << std::endl;
      upper_tree = compute_level_set<V, upper<V> >(ima, upp_reg);

      //draw_tree(ima, upper_tree);

      std::cout << "3/ Merge the two trees." << std::endl;

      // FIXME : the algorithm is contrast invariant.
      //         -> the both calls have to give the same result
      //         -> check it.
      // FIXME : call merge_tree one time will be enough.
      std::cout << "upp_reg = " << &upp_reg << std::endl;
      std::cout << "low_reg = " << &low_reg << std::endl;

      //fllt_tree(P, V) result_tree = merge_trees(upper_tree, lower_tree, upp_reg, low_reg, ima);
      fllt_tree(P, V) result_tree = merge_trees(lower_tree, upper_tree, low_reg, upp_reg, ima);

      return result_tree;
    }

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
