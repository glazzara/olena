// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_TREE_HH
# define MLN_UTIL_TREE_HH

# include <vector>
# include <algorithm>
# include <ostream>
# include <algorithm>
# include <mln/core/contract.hh>

/*!
 * \file   mln/util/tree.hh
 *
 * \brief  Definition of a generic general tree.
 *
 */

namespace mln
{

  namespace util
  {

    /// Fwd declarations.
    template <typename T> class tree_node;
    template <typename T> class tree;
    template <typename T> class branch;


    /*! \brief Class of generic tree_node for tree.
     *
     */
    template <typename T>
    class tree_node
    {
    public:

      typedef std::vector< tree_node<T>* > children_t;

      /*! \brief Constructor.
       *
       */
      tree_node();

      /*! \brief Constructor.
       *
       * \param[in] elt The element of tree_node.
       */
      tree_node(T elt);


      /*! \brief The getter of the element.
       *
       * \return The element of the tree_node.
       */
      T& elt();

      /*! \brief The const getter of the element.
       *
       * \return The element of the tree_node in const.
       */
      const T& elt() const;


      /*! \brief The getter of the children.
       *
       * \return The children of the tree_node.
       */
      children_t& children();


      /*! \brief The getter of the children.
       *
       * \return The children of the tree_node in const.
       */
      const children_t& children() const;


      /*! \brief The getter of the parent.
       *
       * \return The parent of the tree_node.
       */
      tree_node<T>* parent();


      /*! \brief Create a tree_node with \p elt which become the child of
       *  the current tree_node.
       *
       * \param[in] elt The element of the new child to add.
       *
       * \return The new tree_node created.
       */
      tree_node<T>* add_child(T elt);

      /*! \brief Bind \p tree_node to the current tree_node and become its
       *  child.
       *
       * \param[in] tree_node The new child tree_node.
       *
       * \return The child tree_node.
       */
      tree_node<T>* add_child(tree_node<T>* tree_node);

      /*! \brief Bind \p tree_node to the current tree_node and become its
       *  parent.
       *
       * \param[in] parent The new parent tree_node.
       *
       */
      void set_parent(tree_node<T>* parent);

      /*! \brief Delete the current tree_node.
       *
       */
      tree_node<T>* delete_tree_node();

      /*! \brief Print on \p ostr the arborescence with the current
       *  tree_node as root.
       *
       * \param[in] ostr The output stream.
       * \param[in] level The deep level
       *
       */
      void print(std::ostream& ostr, int level = 0);

      /*! \brief Check the consistency of the tree_node.
       *
       *  \return true if no error, else false.
       */
      bool check_consistency();


      /*! \brief Search the tree_node with value \p elt in the arborescence
       *  of the current tree_node.
       *
       *  \param[in] elt The value of the searched tree_node.
       *
       *  \return If not found 0 else the tree_node with \p elt value.
       */
      tree_node<T>* search(T& elt);

      /// The using method for method search.
      int  search_rec(tree_node<T>** res, T& elt);

    private:

      /// The value.
      T elt_;

      /// The tree_node parent.
      tree_node<T>* parent_;

      /// The children.
      std::vector< tree_node<T>* > child_;
    };



    /*! \brief Class of generic tree.
     *
     */
    template <typename T>
    class tree
    {
    public:

      typedef tree_node<T> tree_node_t;

      /*! \brief Constructor.
       *
       */
      tree();

      /*! \brief Constructor.
       *
       * \param[in] root The root of the tree.
       */
      tree(tree_node<T>* root);


      /*! \brief The getter of the root.
       *
       * \return The root's tree_node of the the current tree.
       */
      tree_node<T>* root();

      /*! \brief Convert the tree into brach.
       *
       * \return The root's tree_node of the the current tree.
       */
      branch<T> main_branch();

      /*! \brief Check the consistency of the tree.
       *
       *  \return true if no error, else false.
       */
      bool check_consistency();


      /*! \brief Bind a new tree upper the current.
       *
       *  \param[in] elt The new value of the new tree_node of the new tree
       *  add upper the current.
       */
      void add_tree_up (T& elt);

      /*! \brief Bind a new tree downer the current.
       *
       *  \param[in] elt The new value of the new tree_node of the new tree
       *  add downer the current.
       */
      void add_tree_down (T& elt);

    private:

      /// The root's tree_node.
      tree_node<T>* root_;
    };


    /*! \brief Class of generic branch.
     *
     */
    template <typename T>
    class branch
    {
    public:

      /*! \brief Constructor.
       *
       * \param[in] tree The tree of the branch.
       * \param[in] apex The apex of the branch.
       */
      branch(tree<T>& tree, tree_node<T>& apex);

      /*! \brief The getter of the appex.
       *
       *  \return The tree_node appex of the current branch.
       */
      tree_node<T>& apex();

      /*! \brief The getter of the tree.
       *
       *  \return The tree of the current branch.
       */
      tree<T>& util_tree();

    private:
      /// The tree of this branch.
      util::tree<T>& tree_;

      /// The tree_node apex of this branch.
      tree_node<T>& apex_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    tree<T>::tree()
      : root_ (0)
    {
    }

    template <typename T>
    inline
    tree<T>::tree(tree_node<T>* root)
      : root_ (root)
    {
      mln_assertion (root != 0);
    }

    template <typename T>
    inline
    tree_node<T>*
    tree<T>::root()
    {
      return root_;
    }

    template <typename T>
    inline
    branch<T>
    tree<T>::main_branch()
    {
      return branch<T>(*this, *root());
    }

    template <typename T>
    inline
    void
    tree<T>::add_tree_up(T& elt)
    {
      tree_node<T>* n = new tree_node<T> (elt);
      root_->set_parent(n);
      n->children().push_back (root_);
      root_ = n;
    }

    template <typename T>
    inline
    void
    tree<T>::add_tree_down(T& elt)
    {
      tree_node<T>* n = new tree_node<T> (elt);
      root_->child_.push_back (n);
    }


    template <typename T>
    inline
    bool
    tree<T>::check_consistency()
    {
      return root()->check_consistency ();
    }

    template <typename T>
    inline
    tree_node<T>::tree_node()
      : parent_ (0)
    {
    }

    template <typename T>
    inline
    tree_node<T>::tree_node(T elt)
      : elt_ (elt),
	parent_ (0)
    {
    }

    template <typename T>
    inline
    const T&
    tree_node<T>::elt() const
    {
      return elt_;
    }

    template <typename T>
    inline
    T&
    tree_node<T>::elt()
    {
      return elt_;
    }


    template <typename T>
    inline
    std::vector< tree_node<T>* >&
    tree_node<T>::children()
    {
      return child_;
    }

    template <typename T>
    inline
    const std::vector< tree_node<T>* >&
    tree_node<T>::children() const
    {
      return child_;
    }

    template <typename T>
    inline
    tree_node<T>*
    tree_node<T>::add_child(T elt)
    {
      tree_node<T>* s = new tree_node<T>(elt);

      s->parent_ = this;
      this->child_.push_back(s);
      return s;
    }


    template <typename T>
    inline
    tree_node<T>*
    tree_node<T>::add_child(tree_node<T>* tree_node)
    {
      if (tree_node->parent_)
	{
	  for (typename std::vector<util::tree_node<T>* >::iterator it = tree_node->parent()->children().begin();
	       it != tree_node->parent()->children().end(); ++it)
	    if ((*it) == tree_node)
	    {
	      tree_node->parent()->children().erase(it);
	      break;
	    }
	}
      tree_node->parent_ = this;
      this->children().push_back(tree_node);
      return tree_node;
    }

    template <typename T>
    inline
    tree_node<T>*
    tree_node<T>::delete_tree_node()
    {
      mln_assertion(parent_ != 0);
      tree_node<T>* res = parent_;

      typename std::vector<tree_node<T>* >::iterator it = parent_->children().begin();
      for (; it < parent_->children().end(); ++it)
	if ((*it) == this)
	  {
	    parent_->children().erase(it);
	    break;
	  }

      for (typename std::vector<tree_node<T>* >::iterator it = this->child_.begin();
	   it != this->child_.end(); ++it)
	parent_->add_child(*it);
      return (res);
    }

    template <typename T>
    inline
    void
    tree_node<T>::print(std::ostream& ostr, int level)
    {
      ostr << level << std::endl;

      ostr << " elt " << this->elt() << std::endl;


      for (typename std::vector<tree_node<T>* >::iterator it = this->child_.begin();
	   it != this->child_.end(); ++it)
	{
	  (*it)->print(level + 1);
	}
    }


    template <typename T>
    inline
    void
    tree_node<T>::set_parent(tree_node<T>* parent)
    {
      mln_assertion(parent != 0);
      parent_ = parent;
      parent->child_.push_back(this);
    }

    template <typename T>
    inline
    tree_node<T>*
    tree_node<T>::parent()
    {
      return parent_;
    }

    template <typename T>
    inline
    int
    tree_node<T>::search_rec(tree_node<T>** res, T& elt)
    {
      if (elt == this->elt_)
      {
	*res = this;
	return 1;
      }
      else
      {
	for (typename std::vector<tree_node<T>* >::iterator it = this->child_.begin();
	     it != this->child_.end(); ++it)
	{
	  if ((**it).search_rec(res, elt))
	    return 1;
	}
      }
      return 0;
    }

    template <typename T>
    inline
    tree_node<T>*
    tree_node<T>::search(T& elt)
    {
      tree_node<T>* res = 0;

      if (search_rec(&res, elt))
	return res;
      return 0;
    }

    template <typename T>
    inline
    bool
    tree_node<T>::check_consistency()
    {
      for (typename std::vector<tree_node<T>* >::iterator it = this->child_.begin();
	   it != this->child_.end(); ++it)
      {
	if ((**it).parent() != this)
	  return false;

	if (!((**it).check_consistency()))
	  return false;
      }
      return true;
    }


    // Branch methods
    template <typename T>
    inline
    branch<T>::branch(util::tree<T>& tree,
		      util::tree_node<T>& apex)
      : tree_(tree),
	apex_(apex)
    {
    }


    template <typename T>
    inline
    util::tree_node<T>&
    branch<T>::apex()
    {
      return apex_;
    }

    template <typename T>
    inline
    mln::util::tree<T>&
    branch<T>::util_tree()
    {
      return tree_;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util


} // end of namespace mln


#endif // !MLN_UTIL_TREE_HH
