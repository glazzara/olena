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

/// \file
///
/// Compute a canonized (parenthood) component tree from a dual input.


#ifndef MLN_MORPHO_TREE_DUAL_INPUT_TREE_HH
# define MLN_MORPHO_TREE_DUAL_INPUT_TREE_HH

# include <mln/data/sort_psites.hh>
# include <mln/morpho/tree/data.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename I, typename M, typename N>
      inline
      morpho::tree::data< I, p_array<mln_psite(I)> >
	dual_input_max_tree(const Image<I>& f,
			    const Image<M>& m,
			    const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename M, typename N>
      inline
      morpho::tree::data< I, p_array<mln_psite(I)> >
      dual_input_max_tree(const Image<I>& f_,
			  const Image<M>& m_,
			  const Neighborhood<N>& nbh_)
      {
	trace::entering("morpho::tree::dual_input_max_tree");

	const I& f = exact(f_);
	const M& m = exact(m_);
	const N& nbh = exact(nbh_);

	mln_precondition(f.is_valid());
	mln_precondition(m.is_valid());
	mln_precondition(nbh.is_valid());

	typedef p_array<mln_psite(I)> S;
	typedef data<I,S> tree_t;

	S s_f = mln::data::sort_psites_increasing(f);
	S s_m = mln::data::sort_psites_increasing(m);
	tree_t tree(f, m, s_f, s_m, nbh);

	trace::exiting("morpho::tree::dual_input_max_tree");
	return tree;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // !MLN_MORPHO_TREE_DUAL_INPUT_TREE_HH
