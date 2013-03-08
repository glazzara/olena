// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_UTIL_BRANCH_ITER_HH
# define MLN_UTIL_BRANCH_ITER_HH

/*!
 * \file
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

    /*!
      \internal
      \brief Iterator on branch.

      The parameter \c T is the type of node's data. branch_iter is
      used to pre-order walk a branch.

    */
    template <typename T>
    class branch_iter
    {
    public:
      branch_iter(branch<T> branch);

      /// Conversion to node.
      operator util::tree_node<T>&() const;
      util::tree_node<T>& operator *();

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

      typedef typename std::vector< util::tree_node<T>* >::iterator child_iter;
      typedef std::pair<child_iter, child_iter> iter_pair;
      /// Store child().begin() and child().end().
      std::stack< iter_pair > s_;

      util::tree_node<T>* n_;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename T>
    inline
    branch_iter<T>::branch_iter(branch<T> branch)
      : branch_(branch)
    {
      invalidate();
    }

    template <typename T>
    inline
    branch_iter<T>::operator util::tree_node<T>&() const
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    inline
    util::tree_node<T>&
    branch_iter<T>::operator*()
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    inline
    unsigned
    branch_iter<T>::deepness() const
    {
      mln_assertion(is_valid());
      unsigned i = 0;
      tree_node<T>* p = n_;
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
    branch_iter<T>::is_valid() const
    {
      return n_ != 0;
    }

    template <typename T>
    inline
    void
    branch_iter<T>::invalidate()
    {
      n_ = 0;
    }


    template <typename T>
    inline
    void
    branch_iter<T>::start()
    {
      s_.push(iter_pair(branch_.apex().children().begin(),
			branch_.apex().children().end()));
      n_ = &branch_.apex();
    }

    template <typename T>
    inline
    void
    branch_iter<T>::next()
    {
      if (s_.size() == 0)
	invalidate();
      else
      {
	if (s_.top().first == s_.top().second)
	{
	  s_.pop();
	  next();
	  return;
	}
	else
	{
	  n_ = *(s_.top().first);
	  s_.top().first++;

	  if (!n_)
	  {
	    next();
	    return;
	  }

	  mln_assertion(n_);
	  if (n_->children().size() > 0)
	  {
	    s_.push(iter_pair(n_->children().begin(),
			      n_->children().end()));
	  }
	  return;
	}
      }
    }

# endif // ! MLN_INCLUDE_ONLY


  }

} // end of namespace mln


#endif // ! MLN_UTIL_BRANCH_ITER_HH
