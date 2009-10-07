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

#ifndef MLN_UTIL_CTREE_INTERNAL_TREE_BASE_HH
# define MLN_UTIL_CTREE_INTERNAL_TREE_BASE_HH

# include <mln/core/concept/object.hh>
# include <mln/core/concept/tree.hh>
# include <mln/core/concept/proxy.hh>
# include <mln/core/internal/data.hh>

# include <mln/core/site_set/p_run_idx.hh>

# include <mln/util/ctree/node.hh>
# include <mln/util/tracked_ptr.hh>

namespace mln
{

  namespace util
  {

    namespace ctree
    {

      namespace internal
      {

	template <typename I, typename E>
	class tree_base : public Tree<E>
	{
	public:
	  /// Site related type definitions.
	  /// \{
	  /// The type of site.
	  typedef mln_site(I) site;
	  /// The type of pseudo-site.
	  typedef mln_psite(I) psite;
	  /// The type of domain.
	  typedef mln_domain(I) domain_t;
	  /// \}

	  /// Node related type definions.
	  /// \{
	  /// The type of the node.
	  typedef ctree::node<E> node_t;
	  /// The type of node set.
	  typedef p_run_idx<node_t> nodes_t;
	  /// \}

	  /// Node/Site value related type definitions.
	  /// \{
	  /// The type of node value
	  typedef mln_value(I) value;
	  /// The type of node rvalue.
	  typedef mln_rvalue(I) rvalue;
	  /// The type of node lvalue.
	  typedef mln_lvalue(I) lvalue;
	  /// \}

	  /// Check validity.
	  bool is_valid() const;
	  void invalidate();

	  /// Hook to data; for debugging purpose.
	  const util::tracked_ptr< mln::internal::data<E> >& data_hook_() const;

	public: // Tmp for construction.
	  util::tracked_ptr< mln::internal::data<E> > data_;
	protected:
	  tree_base();
	};

# ifndef MLN_INCLUDE_ONLY

	template<typename I, typename E>
	inline
	tree_base<I,E>::tree_base()
	  : data_ (0)
	{
	}

	template <typename I, typename E>
	inline
	bool
	tree_base<I,E>::is_valid() const
	{
	  return data_ != 0;
	}

	template <typename I, typename E>
	inline
	void
	tree_base<I,E>::invalidate()
	{
	  data_.clean_();
	}

	template<typename I, typename E>
	inline
	const util::tracked_ptr< mln::internal::data<E> >&
	tree_base<I,E>::data_hook_() const
	{
	  return data_;
	}


# endif // ! MLN_INCLUDE_ONLY

      }  // end of namespace mln::util::ctree::internal

    } // end of namespace mln::util::ctree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_UTIL_CTREE_INTERNAL_TREE_BASE_HH
