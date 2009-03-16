// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_TREE_DATA_HH
# define MLN_MORPHO_TREE_DATA_HH

/// \file mln/morpho/tree/data.hh
///
/// FIXME: First Attempt.
///
/// \todo Fix the issue pointed to by Edwin without modifying the way
/// sites are browsed (see the documentation of compute_parent to
/// learn why we want the 1st pass to be in forward scan of s).

# include <mln/morpho/tree/compute_parent.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/internal/piter_identity.hh>
# include <deque>

# define mln_up_site_piter(T)	typename T::up_site_piter
# define mln_dn_site_piter(T)	typename T::dn_site_piter
# define mln_up_node_piter(T)	typename T::up_node_piter
# define mln_dn_node_piter(T)	typename T::dn_node_piter
# define mln_preorder_piter(T)	typename T::preorder_piter

# define mln_up_site_piter_(T)	T::up_site_piter
# define mln_dn_site_piter_(T)  T::dn_site_piter
# define mln_up_node_piter_(T)  T::up_node_piter
# define mln_dn_node_piter_(T)  T::dn_node_piter
# define mln_preorder_piter_(T) T::preorder_piter

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

      /// Iterate on tree's nodes (component representants) from leaves to roots.
      template <typename T> struct up_node_piter;

      /// Iterate on tree's nodes (component representants) from leaves to roots.
      template <typename T> struct dn_node_piter;

      /// Preorder tree traversal iterator.
      template <typename T> struct preorder_piter;

      /// Postorder tree traversal iterator.
      //template <typename T> struct postorder_piter;

      // FIXME: Doc!



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
	/// Node list associated type.
	typedef p_array<mln_psite(I)> nodes_t;

	/// Parent image associated type.
	typedef mln_ch_value(I, mln_psite(I)) parent_t;

	// Iterate on all sites.
	typedef mln::morpho::tree::up_site_piter<self_> up_site_piter;
	typedef mln::morpho::tree::dn_site_piter<self_> dn_site_piter;

	// Iterate on nodes only.
	typedef mln::morpho::tree::up_node_piter<self_> up_node_piter;
	typedef mln::morpho::tree::dn_node_piter<self_> dn_node_piter;

	typedef mln::morpho::tree::preorder_piter<self_> preorder_piter;

// 	typedef mln_bkd_piter(S) piter;
// 	typedef mln_bkd_piter(S) fwd_piter;
// 	typedef mln_fwd_piter(S) bkd_piter;

	/// Constructor.
	template <typename N>
	explicit data(const Image<I>& f, const Site_Set<S>& s, const Neighborhood<N>& nbh);



	/// \{ Parent-related materials.

	const mln_psite(I)& parent(const mln_psite(I)& p) const;
	const parent_t& parent_image() const;

	/// \}

	const p_array<mln_psite(I)>& children(const mln_psite(I)& p) const;
	const mln_ch_value(I, nodes_t)& children_image() const;

	/// \{ Tests.

	bool is_valid() const;
	bool is_root(const mln_psite(I)& p) const;
	bool is_a_node(const mln_psite(I)& p) const;
	bool is_a_non_root_node(const mln_psite(I)& p) const;
	bool is_a_leaf(const mln_psite(I)& p) const;

	/// \}


	/// \{ Nodes-related materials.

	const p_array<mln_psite(I)>& nodes() const;

	/// \}

	/// \{ Sites-related materials.

	const S& domain() const;

	/// \}

	unsigned nroots() const;


	const I& f() const;


	mln_rvalue(I) f(const mln_psite(I)& p) const;


      protected:
	const function& f_;
	const sites_t& s_;
	mln_ch_value(I, mln_psite(I)) parent_;	// Parent image.
	mln_ch_value(I, nodes_t) children_;	// Children image.
	nodes_t nodes_;
	unsigned nroots_;
      };


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
	typedef typename T::sites_t::fwd_piter Pi_;
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
	typedef typename T::sites_t::bkd_piter Pi_;
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
      class preorder_piter
	: public mln::internal::site_set_iterator_base< T, preorder_piter<T> >
      {
	typedef preorder_piter<T> self_;
	typedef mln::internal::site_set_iterator_base<T, self_> super_;

      public:

	/// Constructor with no argument.
	preorder_piter();

	/// Constructor.
	preorder_piter(const T& t);


	preorder_piter(const T& t,
		       const mln_psite(T::function)& p);

	/// Test if the iterator is valid.
	bool is_valid_() const;

	/// Invalidate the iterator.
	void invalidate_();

	/// Start an iteration.
	void start_();

	/// Go to the next point.
	void next_();

      protected:
	using super_::p_;
	using super_::s_;
	std::deque<mln_psite(T::function)> stack_; // FIXME: implement p_stack.
	const mln_psite(T::function)* root_;
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
	initialize(children_, f);

	// Store tree nodes.
	nroots_ = 0;
	mln_bkd_piter(S) p(s_);
	for_all(p)
	{
	  if (f_(parent_(p)) != f_(p))
	    {
	      nodes_.insert(p);
	      children_(parent_(p)).insert(p);
	    }
	  else
	    if (parent_(p) == p)
	      {
		nodes_.insert(p);
		++nroots_;
	      }
	}
      }

      template <typename I, typename S>
      inline
      const mln_psite(I)&
      data<I,S>::parent(const mln_psite(I)& p) const
      {
	mln_precondition(is_valid());
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
      const p_array<mln_psite(I)>&
      data<I,S>::children(const mln_psite(I)& p) const
      {
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
      preorder_piter<T>::preorder_piter()
	: root_ (0)
      {
      }

      template <typename T>
      inline
      preorder_piter<T>::preorder_piter(const T& t)
	: root_ (0)
      {
	this->change_target(t);
      }

      template <typename T>
      inline
      preorder_piter<T>::preorder_piter(const T& t,
					const mln_psite(T::function)& p)
	: root_ (&p)
      {
	this->change_target(t);
      }

      template <typename T>
      inline
      bool
      preorder_piter<T>::is_valid_() const
      {
	return !stack_.empty();
      }

      template <typename T>
      inline
      void
      preorder_piter<T>::invalidate_()
      {
	stack_.clear();
      }

      template <typename T>
      inline
      void
      preorder_piter<T>::start_()
      {
	this->invalidate();
	stack_.push_back(mln_psite(T::function)()); // needed for last element.

	if (root_)
	  stack_.push_back(*root_);
	else
	  {
	    mln_dn_node_piter(T) n(*s_);
	    int roots = 0;
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
      preorder_piter<T>::next_()
      {
	p_ = stack_.back();
	stack_.pop_back();
	if (!exact(this)->is_valid())
	  return;
	mln_fwd_piter(T::nodes_t) child(s_->children(p_));
	for_all(child)
	  stack_.push_back(child);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_TREE_DATA_HH
