// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_MAX_HH
# define MLN_MORPHO_TREE_MAX_HH

/// \file
///
/// Compute a canonized (parenthood) max-tree from an image.

# include <mln/morpho/tree/compute_parent.hh>
# include <mln/data/sort_psites.hh>


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      // Remember:
      //   p is root iff parent(p) == p
      //   p is node iff either p is root or f(parent(p)) != f(p)

      template <typename I, typename N>
      mln_ch_value(I, mln_psite(I))
      max(const Image<I>& f, const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename N>
      inline
      mln_ch_value(I, mln_psite(I))
      max(const Image<I>& f_, const Neighborhood<N>& nbh_)
      {
	mln_trace("morpho::tree::max");

	const I& f   = exact(f_);
	const N& nbh = exact(nbh_);

	mln_precondition(f.is_valid());
	mln_precondition(nbh.is_valid());

	// For the max-tree, childhood maps "increasing level":
	p_array<mln_psite(I)> s = mln::data::sort_psites_increasing(f);
	mln_ch_value(I, mln_psite(I)) output = compute_parent(f, nbh, s);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::morpho::tree

  }  // end of namespace mln::morpho

}  // end of namespace mln


#endif // ! MLN_MORPHO_TREE_MAX_HH
