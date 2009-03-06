// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef ROUTINES_HH_
# define ROUTINES_HH_

# include <mln/morpho/tree/data.hh>
# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>

namespace mln {
  namespace morpho {
    namespace tree {

      template <typename I, typename T>
      util::array< mln_psite(I) >
      get_first_nodes(const Image<I>& img_, const T& tree)
      {
	I bin = exact(img_);
	mln_ch_value(I, bool) deja_vu;
	util::array< mln_psite(I) > fnodes;

	initialize(deja_vu, img_);
	data::fill(deja_vu, false);

	bool can_break = false;
	mln_bkd_piter(T) p(tree.nodes());
	for_all(p)
	{
	  mln_assertion(tree.is_a_node(p));
	  if (bin(p) && !bin(tree.parent(p)))
	    {
	      fnodes.append(p);
	    }
	}
	return fnodes;
      }

    }
  }
}


#endif /* !ROUTINES_HH_ */
