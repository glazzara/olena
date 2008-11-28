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

#ifndef MLN_MORPHO_TREE_UTILS_HH
# define MLN_MORPHO_TREE_UTILS_HH

/// \file mln/morpho/tree/utils.hh
///
/// Utilities.
///
/// \todo Remove this file after ./data.hh is completed!

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename T, typename I>
      bool
      is_root(const Image<T>& parent, const mln_psite(T)& p,
	      const Image<I>& f);


      template <typename T, typename I>
      bool
      is_a_node(const Image<T>& parent, const mln_psite(T)& p,
		const Image<I>& f);



      template <typename T, typename I, typename S>
      p_array<mln_psite(T)>
      nodes(const Image<T>& parent, const Image<I>& f, const Site_Set<S>& s);


# ifndef MLN_INCLUDE_ONLY


      template <typename T, typename I>
      inline
      bool
      is_root(const Image<T>& parent_, const mln_psite(T)& p,
	      const Image<I>& f_)
      {
	mlc_equal(mln_value(T), mln_psite(T))::check();

	const T& parent = exact(parent_);
	const I& f = exact(f_);

	mln_precondition(parent.has_data());
	mln_precondition(f.has_data());
	mln_precondition(parent.domain() == f.domain());

	return parent(p) == p;
      }


      template <typename T, typename I>
      inline
      bool
      is_a_node(const Image<T>& parent_, const mln_psite(T)& p,
		const Image<I>& f_)
      {
	mlc_equal(mln_value(T), mln_psite(T))::check();

	const T& parent = exact(parent_);
	const I& f = exact(f_);

	mln_precondition(parent.has_data());
	mln_precondition(f.has_data());
	mln_precondition(parent.domain() == f.domain());

	return parent(p) == p || f(parent(p)) != f(p);
      }


      template <typename T, typename I, typename S>
      inline
      p_array<mln_psite(T)>
      nodes(const Image<T>& parent_, const Image<I>& f_, const Site_Set<S>& s_)
      {
	mlc_equal(mln_value(T), mln_psite(T))::check();

	const T& parent = exact(parent_);
	const I& f = exact(f_);
	const S& s = exact(s_);

	mln_precondition(parent.has_data());
	mln_precondition(f.has_data());
	mln_precondition(f.domain() == parent.domain());
	mln_precondition(s == f.domain());

	p_array<mln_psite(T)> arr;
	mln_bkd_piter(S) p(exact(s));
	for_all(p)
	  if (is_a_node(parent, p, f))
	    arr.insert(p);

	return arr;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::morpho::tree

  }  // end of namespace mln::morpho

}  // end of namespace mln


#endif // ! MLN_MORPHO_TREE_UTILS_HH
