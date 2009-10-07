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

#ifndef MLN_UTIL_CTREE_NODE_HH
# define MLN_UTIL_CTREE_NODE_HH

# include <mln/core/internal/index_site_base.hh>

namespace mln
{

  /// Node category flag type.
  template <typename E> struct Node {};

  template <>
  struct Node<void>
  {
    typedef Index_Site<void> super;
  };

  namespace util
  {

    namespace ctree
    {

      template <typename T>
      class node : public internal::index_site_base< node<T> >
      {
      public:
	/// Object category.
	typedef Node<void> category;

	typedef internal::index_site_base<node> super;

	typedef typename super::index_t index_t;

	/// Constructors
	/// \{
	node();
	node(const Tree<T>& tree);
	node(const Tree<T>& tree, const index_t& id);
	/// \}

	/// Return whether the node is valid.
	const T& tree() const;
	void change_target(const Tree<T>& tree);

    private:
	const T* tree_;
      };

    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

  namespace internal
  {

    /// subject_impl specialization (Proxy)
    /// \{

    template <typename T, typename E>
    struct subject_impl< const util::ctree::node<T>, E > :
      subject_impl< const internal::index_site_base< util::ctree::node<T> > , E >
    {
      const T& tree() const;

    private:
      const E& exact_() const;
    };

    template <typename T, typename E>
    struct subject_impl< util::ctree::node<T>, E > :
      subject_impl< const util::ctree::node<T>, E >,
      subject_impl< internal::index_site_base< util::ctree::node<T> > , E >
    {
    };

    /// \}

  } // end of namespace mln::internal

# ifndef MLN_INCLUDE_ONLY

  namespace util
  {

    namespace ctree
    {

      template <typename T>
      inline
      node<T>::node()
	: tree_ (0)
      {
      }

      template <typename T>
      inline
      node<T>::node(const Tree<T>& tree)
	: tree_ (&(exact(tree)))
      {
      }


      template <typename T>
      inline
      node<T>::node(const Tree<T>& tree, const index_t& i)
	: super(i),
	  tree_ (&(exact(tree)))
      {
      }

      template <typename T>
      inline
      const T&
      node<T>::tree() const
      {
	return tree_;
      }

      template <typename T>
      inline
      void
      node<T>::change_target(const Tree<T>& tree)
      {
	tree_ = &(exact(tree));
      }


    } // end of namespace mln::util::ctree

  } // end of namespace mln::util

  namespace internal
  {

    template <typename T, typename E>
    inline
    const E&
    subject_impl< const util::ctree::node<T>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    template <typename T, typename E>
    inline
    const T&
    subject_impl< const util::ctree::node<T>, E >::tree() const
    {
      return exact_().get_subject().tree();
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !MLN_UTIL_CTREE_NODE_HH
