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

#ifndef MLN_UTIL_BRANCH_ITER_IND_HH
# define MLN_UTIL_BRANCH_ITER_IND_HH

/*!
 * \file   mln/util/branch_iter_ind.hh
 *
 * \brief  Definition of a iterator on branch.
 *
 */

# include <stack>
# include <mln/util/tree.hh>

namespace mln
{

  namespace util
  {
    template<typename T>
    struct bi_elt
    {
      typedef std::vector< util::node<T>* > child_list;

      bi_elt(child_list* list)
	: list_(list),
	  previous_(0),
	  pos_(-1) {}

      child_list* list_;
      util::node<T>* previous_;
      int pos_;
    };

    /*! \brief Basic 2D image class.
     *
     * The parameter \c T is the type of node's data. branch_iter_ind
     * is used to pre-order walk a branch.
     *
     */
    template <typename T>
    class branch_iter_ind
    {
    public:
      branch_iter_ind(branch<T> branch);

      /// Convertion to node.
      operator util::node<T>&() const;
      util::node<T>& operator *();

      /// Test the iterator validity.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Start an iteration.
      void start();

      /// Go to the next point.
      void next();

      /// Give how deep is the iterator in the branch.
      unsigned deepness() const;

    private:
      /// The branch to iter.
      util::branch<T> branch_;

      /// Store child().begin() and child().end().
      std::stack< bi_elt<T> > s_;

      util::node<T>* n_;
    };

# ifndef MLN_INCLUDE_ONLY


    template <typename T>
    inline
    branch_iter_ind<T>::branch_iter_ind(branch<T> branch)
      : branch_(branch)
    {
      invalidate();
    }

    template <typename T>
    inline
    branch_iter_ind<T>::operator node<T>&() const
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    inline
    util::node<T>&
    branch_iter_ind<T>::operator*()
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    inline
    unsigned
    branch_iter_ind<T>::deepness() const
    {
      mln_assertion(is_valid());
      unsigned i = 0;
      node<T>* p = n_;
      while (p)
      {
	p = p->parent();
	i++;
      }
      return i;
    }

    template <typename T>
    inline
    bool
    branch_iter_ind<T>::is_valid() const
    {
      return n_ != 0;
    }

    template <typename T>
    inline
    void
    branch_iter_ind<T>::invalidate()
    {
      n_ = 0;
    }


    template <typename T>
    inline
    void
    branch_iter_ind<T>::start()
    {
      s_.push(bi_elt<T>(&branch_.apex().children()));

      n_ = &branch_.apex();
    }

    template <typename T>
    inline
    void
    branch_iter_ind<T>::next()
    {
      // First : list of children.
      // Second : i;

      if (s_.size() == 0)
	invalidate();
      else
      {
	s_.top().pos_++;
	if (s_.top().list_->size() == (unsigned)s_.top().pos_)
	{
	  s_.pop();
	  next();
	  return;
	}
	else
	{
	  mln_assertion(s_.top().list_->size() > (unsigned)s_.top().pos_);
	  if (s_.top().previous_ != 0)
	    mln_assertion(s_.top().previous_ == (*(s_.top().list_))[s_.top().pos_ - 1]);

	  n_ = (*(s_.top().list_))[s_.top().pos_];
	  s_.top().previous_ = n_;

	  if (!n_)
	  {
	    next();
	    return;
	  }

	  mln_assertion(n_);
	  if (n_->children().size() > 0)
	  {
	    s_.push(bi_elt<T>(&n_->children()));
	  }
	  return;
	}
      }
    }

# endif // ! MLN_INCLUDE_ONLY


  }

} // end of namespace mln


#endif // !MLN_UTIL_BRANCH_ITER_IND_HH
