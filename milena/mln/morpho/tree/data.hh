// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_DATA_HH
# define MLN_MORPHO_TREE_DATA_HH

/// \file
///
/// \todo Think about site iterator (using image site iterator instead
/// of S container iterator) to go faster.

# include <mln/morpho/tree/compute_parent.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/internal/piter_identity.hh>

# include <deque>
# include <iostream>

# define mln_up_site_piter(T)	typename T::up_site_piter
# define mln_dn_site_piter(T)	typename T::dn_site_piter
# define mln_up_node_piter(T)	typename T::up_node_piter
# define mln_dn_node_piter(T)	typename T::dn_node_piter
# define mln_up_leaf_piter(T)	typename T::up_leaf_piter
# define mln_dn_leaf_piter(T)	typename T::dn_leaf_piter
# define mln_site_piter(T)	typename T::site_piter
# define mln_node_piter(T)	typename T::node_piter
# define mln_leaf_piter(T)	typename T::leaf_piter
# define mln_depth1st_piter(T)	typename T::depth1st_piter

# define mln_up_site_piter_(T)	T::up_site_piter
# define mln_dn_site_piter_(T)  T::dn_site_piter
# define mln_up_node_piter_(T)  T::up_node_piter
# define mln_dn_node_piter_(T)  T::dn_node_piter
# define mln_up_leaf_piter_(T)	T::up_leaf_piter
# define mln_dn_leaf_piter_(T)	T::dn_leaf_piter
# define mln_site_piter_(T)	T::site_piter
# define mln_node_piter_(T)	T::node_piter
# define mln_leaf_piter_(T)	T::leaf_piter
# define mln_depth1st_piter_(T) T::depth1st_piter


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      // Forward declarations.

      /// Iterate on tree's sites from leaves to roots.
      template <typename T> struct up_site_piter;

      /// Iterate on tree's sites from roots to leaves.
      template <typename T> struct dn_site_piter;

      /// Iterate on tree's nodes (component representants) from
      /// leaves to roots.
      template <typename T> struct up_node_piter;

      /// Iterate on tree's nodes (component representants) from
      /// leaves to roots.
      template <typename T> struct dn_node_piter;

      /// Iterate on tree's leaves in the same way of up_node_piter.
      template <typename T> struct up_leaf_piter;

      /// Iterate on tree's leaves in the same way of dn_node_piter.
      template <typename T> struct dn_leaf_piter;

      /// Depth1st tree traversal iterator.
      template <typename T> class depth1st_piter;


      template <typename I, typename S>
      class data
      {
	typedef data<I, S> self_;

      public:
	/// Associated type of the function f.
	typedef I function;
	/// Psite associated type.
	typedef mln_psite(I) psite;
	typedef mln_site(I) site;
	/// Site set associated type.
	typedef S sites_t;
	/// Node set associated type.
	typedef p_array<mln_psite(I)> nodes_t;
	/// Leaf set associated type.
	typedef p_array<mln_psite(I)> leaves_t;

	/// Parent image associated type.
	typedef mln_ch_value(I, mln_psite(I)) parent_t;

	/// Children image associated type
	typedef mln_ch_value(I, nodes_t) children_t;

	// Iterate on all sites.
	typedef mln::morpho::tree::up_site_piter<self_> up_site_piter;
	typedef mln::morpho::tree::dn_site_piter<self_> dn_site_piter;
	typedef up_site_piter site_piter;

	// Iterate on nodes only.
	typedef mln::morpho::tree::up_node_piter<self_> up_node_piter;
	typedef mln::morpho::tree::dn_node_piter<self_> dn_node_piter;
	typedef up_node_piter node_piter;

	// Iterate on leaves only.
	typedef mln::morpho::tree::up_leaf_piter<self_> up_leaf_piter;
	typedef mln::morpho::tree::dn_leaf_piter<self_> dn_leaf_piter;
	typedef up_leaf_piter leaf_piter;

	typedef mln::morpho::tree::depth1st_piter<self_> depth1st_piter;


	/// Standard constructor.
	template <typename N>
	data(const Image<I>& f,
	     const Site_Set<S>& s,
	     const Neighborhood<N>& nbh);

	/// Special constructor where the parent computation has
	/// delegated to an external function. (To handle special case
	/// of connectivity for example).
	data(const Image<I>& f,
	     const parent_t& parent,
	     const Site_Set<S>& s);

	/// \{ Base function-related materials

	mln_rvalue(I) f(const mln_psite(I)& p) const;
	const I& f() const;

	/// \}

	/// \{ Parent-related materials.

	mln_rvalue(parent_t) parent(const mln_psite(I)& p) const;
	const parent_t& parent_image() const;

	/// \}

	/// \{ Child-related materials.

	mln_rvalue(children_t) children(const mln_psite(I)& p) const;
	const mln_ch_value(I, nodes_t)& children_image() const;

	/// \}

	/// \{ Nodes materials.

	const p_array<mln_psite(I)>& nodes() const;

	/// \}

	/// \{ Leaves materials.

	const p_array<mln_psite(I)>& leaves() const;

	/// \}

	/// \{ Sites materials.

	const S& domain() const;

	/// \}



	/// \{ Tests.

	bool is_valid() const;
	bool is_root(const mln_psite(I)& p) const;
	bool is_a_node(const mln_psite(I)& p) const;
	bool is_a_non_root_node(const mln_psite(I)& p) const;
	bool is_a_leaf(const mln_psite(I)& p) const;

	/// \}

	unsigned nroots() const;

      protected:
	void compute_children_();

      protected:
	mln_ch_value(I, mln_psite(I))	parent_;	// Parent image.
	mln_ch_value(I, nodes_t)	children_;	// Children image.

	function f_;      // f image containing values of the tree nodes.
	sites_t s_;       // Sorted site set of the tree sites
		          // (domain(f_) includes s_).

	nodes_t nodes_;   // Sorted node set.
	leaves_t leaves_; // Sorted leaf set.
	unsigned nroots_; // For non-contigous domain image purpose.
      };


      /* Iterators */

      template <typename T>
      struct up_site_piter
	: public mln::internal::piter_identity_< typename T::sites_t::bkd_piter,	// Adaptee.
						 up_site_piter<T> >			// Exact.
      {
      private:
	typedef typename T::sites_t::bkd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, up_site_piter<T> > super_;

      public:
	up_site_piter(const T& t)
	{
	  this->change_target(t.domain());
	}

	up_site_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      struct dn_site_piter
	: public mln::internal::piter_identity_< typename T::sites_t::fwd_piter,	// Adaptee.
						 dn_site_piter<T> >			// Exact.
      {
      private:
	typedef typename T::sites_t::fwd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, dn_site_piter<T> > super_;

      public:
	dn_site_piter(const T& t)
	{
	  this->change_target(t.domain());
	}

	dn_site_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      struct up_node_piter
	: public mln::internal::piter_identity_< typename T::nodes_t::fwd_piter,	// Adaptee.
						 up_node_piter<T> >			// Exact.
      {
      private:
	typedef typename T::nodes_t::fwd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, up_node_piter<T> > super_;

      public:
	up_node_piter(const T& t)
	{
	  this->change_target(t.nodes());
	}

	up_node_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      struct dn_node_piter
	: public mln::internal::piter_identity_< typename T::nodes_t::bkd_piter,	// Adaptee.
						 dn_node_piter<T> >			// Exact.
      {
      private:
	typedef typename T::nodes_t::bkd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, dn_node_piter<T> > super_;

      public:
	dn_node_piter(const T& t)
	{
	  this->change_target(t.nodes());
	}

	dn_node_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      struct up_leaf_piter
	: public mln::internal::piter_identity_< typename T::leaves_t::fwd_piter,	// Adaptee.
						 up_leaf_piter<T> >			// Exact.
      {
      private:
	typedef typename T::leaves_t::fwd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, up_leaf_piter<T> > super_;

      public:
	up_leaf_piter(const T& t)
	{
	  this->change_target(t.leaves());
	}

	up_leaf_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      struct dn_leaf_piter
	: public mln::internal::piter_identity_< typename T::leaves_t::bkd_piter,	// Adaptee.
						 dn_leaf_piter<T> >			// Exact.
      {
      private:
	typedef typename T::leaves_t::bkd_piter Pi_;
	typedef mln::internal::piter_identity_< Pi_, dn_leaf_piter<T> > super_;

      public:
	dn_leaf_piter(const T& t)
	{
	  this->change_target(t.leaves());
	}

	dn_leaf_piter(const Pi_& pi)
	  : super_(pi)
	{
	}
      };

      template <typename T>
      class depth1st_piter
	: public mln::internal::site_set_iterator_base< T, depth1st_piter<T> >
      {
	typedef depth1st_piter<T> self_;
	typedef mln::internal::site_set_iterator_base<T, self_> super_;

      public:

	/// Constructor with no argument.
	depth1st_piter();

	/// Constructor.
	depth1st_piter(const T& t);


	depth1st_piter(const T& t,
		       const mln_psite(T::function)& p);

	/// Test if the iterator is valid.
	bool is_valid_() const;

	/// Invalidate the iterator.
	void invalidate_();

	/// Start an iteration.
	void start_();

	/// Go to the next point.
	void next_();

	/// Skip current point children. Next call to next() goes to
	/// the brother point.
	void skip_children();

      protected:
	using super_::p_;
	using super_::s_;
	std::deque<mln_psite(T::function)> stack_; // FIXME: implement p_stack.
	const mln_psite(T::function)* root_;
      };

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

  template <typename I, typename S>
  inline
  std::ostream&
  operator<< (std::ostream& os, const morpho::tree::data<I, S>& t);


# ifndef MLN_INCLUDE_ONLY

  namespace morpho
  {

    namespace tree
    {

      template <typename I, typename S>
      inline
      data<I, S>::data(const Image<I>& f,
		       const mln_ch_value(I, mln_psite(I))& parent,
		       const Site_Set<S>& s)
	: parent_ (parent),
	  f_ (exact(f)),
	  s_ (exact(s))
      {
	compute_children_();
      }



      template <typename I, typename S>
      template <typename N>
      inline
      data<I,S>::data(const Image<I>& f,
		      const Site_Set<S>& s,
		      const Neighborhood<N>& nbh)
	: f_(exact(f)),
	  s_(exact(s))
      {
	// Compute parent image.
	const N& nbh_ = exact(nbh);
	parent_ = morpho::tree::compute_parent(f_, nbh_, s_);

	compute_children_();
      }

      template <typename I, typename S>
      inline
      void
      data<I, S>::compute_children_()
      {
	initialize(children_, f_);

	// Store tree nodes.
	nroots_ = 0;
	mln_bkd_piter(S) p(s_);
	for_all(p)
	{
	  if (f_(parent_(p)) != f_(p))
	    {
	      nodes_.insert(p);
	      children_(parent_(p)).insert(p);
	      if (is_a_leaf(p))
		leaves_.insert(p);
	    }
	  else if (parent_(p) == p) // It's a root.
	    {
	      nodes_.insert(p);
	      if (is_a_leaf(p)) // One pixel image...
		leaves_.insert(p);
	      ++nroots_;
	    }
	}
	mln_assertion(leaves_.nsites() > 0);
      }


      template <typename I, typename S>
      inline
      mln_rvalue_(mln_ch_value(I, mln_psite(I)))
      data<I,S>::parent(const mln_psite(I)& p) const
      {
	mln_precondition(parent_.domain().has(p));
	return parent_(p);
      }

      template <typename I, typename S>
      inline
      const mln_ch_value(I, mln_psite(I))&
      data<I,S>::parent_image() const
      {
	mln_precondition(is_valid());
	return parent_;
      }

      template <typename I, typename S>
      inline
      bool
      data<I,S>::is_valid() const
      {
	return parent_.is_valid(); // FIXME: and... (?)
      }

      template <typename I, typename S>
      inline
      bool
      data<I,S>::is_root(const mln_psite(I)& p) const
      {
	mln_precondition(is_valid());
	mln_precondition(parent_.domain().has(p));
	return parent_(p) == p;
      }


      template <typename I, typename S>
      inline
      bool
      data<I,S>::is_a_node(const mln_psite(I)& p) const
      {
	mln_precondition(is_valid());
	mln_precondition(parent_.domain().has(p));
	return parent_(p) == p || f_(parent_(p)) != f_(p);
      }

      template <typename I, typename S>
      inline
      bool
      data<I,S>::is_a_non_root_node(const mln_psite(I)& p) const
      {
	mln_precondition(is_valid());
	mln_precondition(parent_.domain().has(p));
	return f_(parent_(p)) != f_(p);
      }


      template <typename I, typename S>
      inline
      bool
      data<I,S>::is_a_leaf(const mln_psite(I)& p) const
      {
	mln_precondition(is_valid());
	mln_precondition(children_.domain().has(p));
	return children_(p).nsites() == 0;
      }

      template <typename I, typename S>
      inline
      const p_array<mln_psite(I)>&
      data<I,S>::nodes() const
      {
	mln_precondition(is_valid());
	return nodes_;
      }

      template <typename I, typename S>
      inline
      const p_array<mln_psite(I)>&
      data<I,S>::leaves() const
      {
	mln_precondition(is_valid());
	return leaves_;
      }

      template <typename I, typename S>
      inline
      const S&
      data<I,S>::domain() const
      {
	return s_;
      }

      template <typename I, typename S>
      inline
      unsigned
      data<I,S>::nroots() const
      {
	return nroots_;
      }

      template <typename I, typename S>
      inline
      const I&
      data<I,S>::f() const
      {
	return f_;
      }

      template <typename I, typename S>
      inline
      mln_rvalue(I)
      data<I,S>::f(const mln_psite(I)& p) const
      {
	return f_(p);
      }

      template <typename I, typename S>
      inline
      mln_rvalue_(mln_ch_value(I, p_array<mln_psite(I)>))
      data<I,S>::children(const mln_psite(I)& p) const
      {
	mln_precondition(is_a_node(p));
	return children_(p);
      }

      template <typename I, typename S>
      inline
      const mln_ch_value(I, p_array<mln_psite(I)>)&
      data<I,S>::children_image() const
      {
	return children_;
      }



      // Iterators.


      template <typename T>
      inline
      depth1st_piter<T>::depth1st_piter()
	: root_ (0)
      {
      }

      template <typename T>
      inline
      depth1st_piter<T>::depth1st_piter(const T& t)
	: root_ (0)
      {
	this->change_target(t);
      }

      template <typename T>
      inline
      depth1st_piter<T>::depth1st_piter(const T& t,
					const mln_psite(T::function)& p)
	: root_ (&p)
      {
	mln_assertion(t.is_a_node(p));
	this->change_target(t);
      }

      template <typename T>
      inline
      bool
      depth1st_piter<T>::is_valid_() const
      {
	return !stack_.empty();
      }

      template <typename T>
      inline
      void
      depth1st_piter<T>::invalidate_()
      {
	stack_.clear();
      }

      template <typename T>
      inline
      void
      depth1st_piter<T>::start_()
      {
	this->invalidate();
	stack_.push_back(mln_psite(T::function)()); // needed for last element.

	if (root_)
	  stack_.push_back(*root_);
	else
	  {
	    mln_dn_node_piter(T) n(*s_);
	    unsigned roots = 0;
	    for (n.start(); n.is_valid() && roots < s_->nroots(); n.next())
	      if (s_->is_root(n))
		{
		  stack_.push_back(n);
		  roots++;
		}
	  }

	this->next();
      }

      template <typename T>
      inline
      void
      depth1st_piter<T>::next_()
      {
	p_ = stack_.back();
	stack_.pop_back();
	if (! this->is_valid())
	  return;

	// mln_invariant(p_.is_valid());
	// std::cout << "children(p).size = " << s_->children(p_).nsites() << std::endl;
	// if (s_->children(p_).nsites() != 0)
	//   {
	//     std::cout << "elt[0] = " << s_->children(p_)[0].to_site().graph().data_hook_() << std::endl;
	//     std::cout << "elt[0] = " << s_->children(p_)[0] << std::endl;
	//   }
	// std::cout << s_->children(p_) << std::endl;

	mln_fwd_piter(T::nodes_t) child(s_->children(p_));
	for_all(child)
	{
	  // std::cout << child.to_site().graph().data_hook_() << std::endl;
	  // mln_invariant(s_->parent(child) == p_);
	  stack_.push_back(child);
	}
      }

      template <typename T>
      inline
      void
      depth1st_piter<T>::skip_children()
      {
	while (stack_.size() != 1 && s_->parent(stack_.back()) == p_)
	  stack_.pop_back();
      }

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

  template <typename I, typename S>
  inline
  std::ostream&
  operator<< (std::ostream& os, const morpho::tree::data<I, S>& t)
  {
    typedef morpho::tree::data<I, S> self_t;

    {
      typedef p_array<mln_psite(self_t)> A;

      mln_ch_value(typename self_t::function, A) content;
      initialize(content, t.f());

      os << "Nodes:\tValue:\tPoints" << std::endl;

      mln_up_site_piter(self_t) p(t);
      for_all(p)
	if (t.is_a_node(p))
	  {
	    os << p << "\t" << t.f(p) << "\t" << content(p) << std::endl;
	    content(p).clear();
	  }
	else
	  content(t.parent(p)).insert(p);
    }

    {
      typename self_t::depth1st_piter n(t);
      mln_psite(self_t) old;

      os << std::endl << "Hierarchy: " << std::endl;
      n.start();

      if (!n.is_valid())
	return os;

      for (old = n, n.next(); n.is_valid(); old = n, n.next())
	{
	  if (old == t.parent(n))
	    os << old << " <- ";
	  else
	    os << old << std::endl
	       << "\t" << t.parent(n) << " <- ";
	}

      os << old << std::endl;
      return os;
    }

  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_MORPHO_TREE_DATA_HH
