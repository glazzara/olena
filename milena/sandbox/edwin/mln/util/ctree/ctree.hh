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

/// FIXME: because there're some stuffs not enough clear...
/// In the previous implementation of the morpho tree, the site set and
/// the domain set had the same type, so that
/// E(nodes) is included in E(sites). In fact, (I think) that's a
/// mistake that comes from the representation using parent image. For
/// real, the tree can be considered as a triplet (node_set, function,
/// relationship) (N, F, R) such that F: node -> (site_set, value) and
/// R: parent node -> node and N: the node set.
///
/// Currently, the node type is not a site but it should
/// be. And so, the set of nodes: nodes_t < Site_Set.
/// We should implement (but I don't know how to make it well) the type
/// Attribute_image < Image that have two kind of sites
/// (node and site of the original image) with
/// overloaded operator(): node -> value
///                        mln_psite(nodes_t) -> value
///
///

/// The differences and what they imply.
/// Attribute_image a; Tree t
/// Node_Set nodes; Site_Set s;
///
/// I'd like to be able to write:
/// a | (subset of t.nodes) as whell as a | (subset of t.domain())

// \todo definition of site, psite is ambigous, replace with content_t
// or other.
// \todo node operator ?



#ifndef MLN_UTIL_CTREE_CTREE_HH
# define MLN_UTIL_CTREE_CTREE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>
# include <mln/util/ctree/internal/tree_base.hh>
# include <algorithm>

namespace mln
{

  /// Forward declaration.
  namespace util { namespace ctree { template <typename I> class ctree; } }

  namespace internal
  {
    /// Data structure for component tree.
    template <>
    template <typename I>
    struct data< util::ctree::ctree<I> >
    {
      typedef util::ctree::ctree<I> T;
      typedef typename T::node_t::index_t index_t;

      data(const I&);
      ~data();

      mln_ch_value(I, index_t)	map_;
      std::vector<index_t>	parent_;
      std::vector<unsigned>	n_nodes_;
      std::vector<mln_value(I)> values_;

      const mln_domain(I)&	domain_;
      typename T::nodes_t	nodes_;
    };

  } // end of namespace mln::internal

  namespace util
  {

    namespace ctree
    {

      template <typename I>
      struct ctree : public internal::tree_base< I, ctree<I> >
      {
	/// The super class.
	typedef internal::tree_base<I, ctree<I> > super;

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
	/// The type of node.
	typedef typename super::node_t node_t;
	/// The type of node set.
	typedef typename super::nodes_t nodes_t;
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

	/// Constructors
	/// \{
	ctree();

	/// Constructor from union-find results.
	/// \param[in] f The input image.
	/// \param[in] parent The (not necessary canonized) parent image.
	/// \param[in] area The number of nodes in each sub-tree.
	/// \param[in] s The sorted site set.
	/// \param[in] nb_nodes The total number of nodes.
	template <typename S>
	ctree(const Image<I>& f,
	      const Site_Set<S>& s,
	      mln_ch_value(I, psite)& parent,
	      mln_ch_value(I, unsigned)& area,
	      unsigned nb_nodes);
	/// \}

	/// Misc.
	/// \{
	const domain_t& domain() const;
	const nodes_t& nodes() const;
	bool has(const psite&) const;
	bool has(const node_t&) const;
	unsigned n_nodes() const;
	/// \}

	/// Value access  material.
	/// \{
	rvalue f(const psite&) const;
	lvalue f(const psite&);
	rvalue f(const node_t&) const;
	lvalue f(const node_t&);
	/// \}

	/// Node relationship material.
	/// \{
	node_t node(const psite&) const;
	void set_node(const psite&, const node_t&);
	node_t parent(const node_t&) const;
	void set_parent(const node_t& n, const node_t& parent);

	/// Return the node set of the sub-tree of \p n.
	nodes_t desc(const node_t& n) const;
	/// Return the node set of ancestors of \p n.
	nodes_t anc(const node_t& n) const;
	/// \}

	/// Node tests material.
	/// \{
	bool is_a_leaf(const node_t&) const;
	bool is_a_root(const node_t&) const;
	/// \}

	/// Fast access material.
	/// \{
	bool has_index(unsigned i) const;
	rvalue f_at_(unsigned i) const;
	lvalue f_at_(unsigned i);
	unsigned parent_at_(unsigned i) const;
	unsigned& parent_at_(unsigned i);
	unsigned node_at_(const psite& p) const;
	unsigned& node_at_(const psite& p);
	/// \}

	/// To attribute image
	attribute_image<ctree<I>, value> to_image() const;

	/// Site -> node function.
	/// \{
	/// Return the map Site -> Node as a V2V function.
	//pw::value< mln_ch_value(I, node_t) > mapper() const;
	/// \}

      };

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    template <typename I>
    inline
    data< util::ctree::ctree<I> >::data(const I& f)
      : domain_ (f.domain())
    {
    }

    template <typename I>
    inline
    data< util::ctree::ctree<I> >::~data()
    {
    }

  } // end of namespace mln::internal

  namespace util
  {

    namespace ctree
    {
      namespace internal
      {
	template <typename T>
	struct create_node : Function_v2v< create_node<T> >
	{
	  typedef typename T::index_t param;
	  typedef typename T::node_t result;

	  create_node(const T& tree)
	    : tree_ (tree)
	  {
	  }

	  result
	  operator()(const param& idx) const
	  {
	    result tmp(tree_, idx);
	    return tmp;
	  }

	private:
	  const T& tree_;
	};
      }


      template <typename I>
      inline
      ctree<I>::ctree()
      {
      }

      template <typename I>
      template <typename S>
      inline
      ctree<I>::ctree(const Image<I>& f_,
		      const Site_Set<S>& s_,
		      mln_ch_value(I, mln_psite(I))& parent,
		      mln_ch_value(I, unsigned)& area,
		      unsigned nb_nodes)
      {
	const I& f = exact(f_);
	const S& s = exact(s_);

	mln_precondition(f.is_valid());
	mln_precondition(s.is_valid());
	mln_precondition(parent.is_valid());
	mln_precondition(area.is_valid());
	mln_precondition(f.domain() == parent.domain());
	mln_precondition(f.domain() == area.domain());
	mln_precondition(f.domain().nsites() == s.nsites());

	this->data_ = new mln::internal::data< ctree<I> >(f);

	initialize(this->data_->map_, f);
	this->data_->parent_.resize(nb_nodes);
	this->data_->n_nodes_.resize(nb_nodes);
	this->data_->values_.resize(nb_nodes);
	this->data_->nodes_.init(node_t(*this, 0), nb_nodes);

	//debug::println("Parent: ", parent);
	//debug::println("Area: ", area);

	unsigned root_offset = 0;
	mln_fwd_piter(S) p(s);
	for_all(p)
	{
	  psite q = parent(p);

	  if (f(parent(q)) == f(q)) // Canonization
	    q = (parent(p) = parent(q));

	  if (f(p) == f(q) && p != q) // Not a node.
	    {
	      this->data_->map_(p) = this->data_->map_(parent(p));
	    }
	  else
	    {
	      unsigned& offset = (p == q) ? root_offset : area(q);

	      // Insert Node.
	      mln_assertion(offset < nb_nodes);
	      this->data_->map_(p) = offset;
	      this->data_->parent_[offset] = this->data_->map_(q);
	      this->data_->values_[offset] = f(p);
	      this->data_->n_nodes_[offset] = area(p);
	      area(p) = offset + 1;
	      offset += this->data_->n_nodes_[offset] + 1;
	    }
	}
      }


      template <typename I>
      inline
      const mln_domain(I)&
      ctree<I>::domain() const
      {
	return this->data_->domain_;
      }

      template <typename I>
      inline
      const typename ctree<I>::nodes_t&
      ctree<I>::nodes() const
      {
	return this->data_->nodes_;
      }

      template <typename I>
      inline
      unsigned
      ctree<I>::n_nodes() const
      {
	mln_precondition(this->is_valid());
	return this->data_->nodes_.nsites();
      }

      template <typename I>
      inline
      bool
      ctree<I>::has(const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	return this->data_->domain_.has(p);
      }

      template <typename I>
      inline
      bool
      ctree<I>::has(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	return n.index() < this->data_->nodes_.nsites();
      }

      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f (const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	return this->data_->values_[this->data_->map_(p)];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f (const mln_psite(I)& p)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	return this->data_->values_[this->data_->map_(p)];
      }

      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f (const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return this->data_->values_[n.index()];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f (const node_t& n)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return this->data_->values_[n.index()];
      }

      template <typename I>
      inline
      typename ctree<I>::node_t
      ctree<I>::node(const mln_psite(I)& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	node_t tmp(*this, this->data_->map_(p));
	mln_postcondition(this->has(tmp));
	return tmp;
      }

      template <typename I>
      inline
      void
      ctree<I>::set_node(const mln_psite(I)& p, const node_t& n)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_precondition(this->has(n));
	this->data_->map_(p) = n.index();
      }


      template <typename I>
      inline
      typename ctree<I>::node_t
      ctree<I>::parent(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	node_t tmp(*this, this->data_->parent_[n.index()]);
	mln_postcondition(tmp.index() < n.index());
	return tmp;
      }

      template <typename I>
      inline
      void
      ctree<I>::set_parent(const node_t& n, const node_t& parent)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	mln_precondition(this->has(parent));
	this->data_->parent_[n.index()] = parent.index();
      }

      template <typename I>
      inline
      typename ctree<I>::nodes_t
      ctree<I>::desc(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	nodes_t tmp(n, this->data_->n_nodes_[n.index()] + 1);
	return tmp;
      }

      template <typename I>
      inline
      bool
      ctree<I>::is_a_root(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return (this->data_->parent_[n.index()] == n.index());
      }

      template <typename I>
      inline
      bool
      ctree<I>::is_a_leaf(const node_t& n) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(n));
	return (this->data_->n_nodes_[n.index()] == 0);
      }

      template <typename I>
      inline
      bool
      ctree<I>::has_index(unsigned i) const
      {
	mln_precondition(this->is_valid());
	return i <= this->n_nodes();
      }

      template <typename I>
      inline
      mln_rvalue(I)
      ctree<I>::f_at_(unsigned i) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->values_[i];
      }

      template <typename I>
      inline
      mln_lvalue(I)
      ctree<I>::f_at_(unsigned i)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	return this->data_->values_[i];
      }

      template <typename I>
      inline
      unsigned
      ctree<I>::parent_at_(unsigned i) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	mln_postcondition(this->has_index(this->data_->parent_[i]));
	return this->data_->parent_[i];
      }

      template <typename I>
      inline
      unsigned&
      ctree<I>::parent_at_(unsigned i)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has_index(i));
	mln_postcondition(this->has_index(this->data_->parent_[i]));
	return this->data_->parent_[i];
      }

       template <typename I>
      inline
      unsigned
      ctree<I>::node_at_(const psite& p) const
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_postcondition(this->has_index(this->data_->map_(p)));
	return this->data_->map_(p);
      }

      template <typename I>
      inline
      unsigned&
      ctree<I>::node_at_(const psite& p)
      {
	mln_precondition(this->is_valid());
	mln_precondition(this->has(p));
	mln_postcondition(this->has_index(this->data_->map_(p)));
	return this->data_->map_(p);
      }

      template <typename I>
      inline
      attribute_image<ctree<I>, mln_value(I)>
      ctree<I>::to_image() const
      {
	mln_precondition(this->is_valid());

	attribute_image<ctree<I>, value> to(*this);
	std::copy(this->data_->values_.begin(), this->data_->values_.end(), to.buffer());
	return to;
      }


    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_UTIL_CTREE_CTREE_HH
