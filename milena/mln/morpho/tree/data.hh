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

#ifndef MLN_MORPHO_TREE_DATA_HH
# define MLN_MORPHO_TREE_DATA_HH

/// \file mln/morpho/tree/data.hh
///
/// FIXME: First Attempt.

# include <mln/morpho/tree/compute_parent.hh>
# include <mln/core/image/sub_image.hh>



namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      // FIXME: Doc!

      template <typename I, typename S>
      class data
      {
      public:


	/// Associated type of the function f.
	typedef I function;


	/// Ctor.
	template <typename N>
	data(const Image<I>& f, const Site_Set<S>& s, const Neighborhood<N>& nbh);



	/// \{ Parent-related materials.

	typedef mln_ch_value(I, mln_psite(I)) parent_t;

	const mln_psite(I)& parent(const mln_psite(I)& p) const
	{
	  mln_precondition(is_valid());
	  mln_precondition(parent_.domain().has(p));
	  return parent_(p);
	}

	const parent_t& parent_image() const
	{
	  mln_precondition(is_valid());
	  return parent_;
	}

	/// \}


	/// \{ Tests.

	bool is_valid() const
	{
	  return parent_.has_data(); // FIXME: and... (?)
	}

	bool is_root(const mln_psite(I)& p) const
	{
	  mln_precondition(is_valid());
	  mln_precondition(parent_.domain().has(p));
	  return parent_(p) == p;
	}

	bool is_a_node(const mln_psite(I)& p) const
	{
	  mln_precondition(is_valid());
	  mln_precondition(parent_.domain().has(p));
	  return parent_(p) == p || f_(parent_(p)) != f_(p);
	}

	/// \}


	/// \{ Nodes-related materials.

	typedef p_array<mln_psite(I)> nodes_t;

	const p_array<mln_psite(I)>& nodes() const
	{
	  mln_precondition(is_valid());
	  return nodes_;
	}

	/// \}


	/// \{ How-to iterate on all sites.

	typedef mln_bkd_piter(S) piter;
	typedef mln_bkd_piter(S) fwd_piter;
	typedef mln_fwd_piter(S) bkd_piter;

	const S& domain() const
	{
	  return s_;
	}
	
	/// \}

	unsigned nroots() const
	{
	  return nroots_;
	}

	const I& f() const
	{
	  return f_;
	}

      protected:

	const I& f_;
	const S& s_;
	mln_ch_value(I, mln_psite(I)) parent_;
	p_array<mln_psite(I)> nodes_;
	unsigned nroots_;
      };




# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename S>
      template <typename N>
      inline
      data<I,S>::data(const Image<I>& f, const Site_Set<S>& s, const Neighborhood<N>& nbh)
	: f_(exact(f)),
	  s_(exact(s))
      {
	const N& nbh_ = exact(nbh);

	// Compute parent image.
	parent_ = morpho::tree::compute_parent(f_, nbh_, s_);
	
	// Store tree nodes.
	nroots_ = 0;
	mln_bkd_piter(S) p(s_);
	for_all(p)
	  if (f_(parent_(p)) != f_(p))
	    nodes_.insert(p);
	  else
	    if (parent_(p) == p)
	      {
		nodes_.insert(p);
		++nroots_;
	      }
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_TREE_DATA_HH
