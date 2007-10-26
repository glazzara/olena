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

#ifndef MLN_UTIL_BRANCH_ITER_HH
# define MLN_UTIL_BRANCH_ITER_HH

# include <stack>
# include <mln/util/tree.hh>
/*!
 * \file   mln/util/branch.hh
 *
 * \brief  Definition of a iterator on branch.
 *
 */

namespace mln
{

  namespace util
  {
    template <typename T>
    class branch_iter
    {
    public:
      branch_iter(branch<T> branch);

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
      util::branch<T> branch_;

      typedef typename std::vector< util::node<T>* >::iterator child_iter;
      typedef std::pair<child_iter, child_iter> iter_pair;
      /// Store child().begin() and child().end().
      std::stack< iter_pair > s_;

      util::node<T>* n_;
      unsigned deepness_;
      unsigned futur_deepness_;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename T>
    branch_iter<T>::branch_iter(branch<T> branch)
      : branch_(branch)
    {
      invalidate();
    }

    template <typename T>
    branch_iter<T>::operator node<T>&() const
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    util::node<T>&
    branch_iter<T>::operator*()
    {
      mln_assertion(n_);
      return *n_;
    }

    template <typename T>
    unsigned
    branch_iter<T>::deepness() const
    {
      mln_assertion(is_valid());
      //return s_.size();

      unsigned i = 0;
      node<T>* p = n_;
      while (p)
      {
	p = p->parent();
	i++;
      }
      return i;
      //      return deepness_;
    }

    template <typename T>
    bool
    branch_iter<T>::is_valid() const
    {
      return n_ != 0;
    }

    template <typename T>
    void
    branch_iter<T>::invalidate()
    {
      n_ = 0;
    }


    template <typename T>
    void
    branch_iter<T>::start()
    {
      deepness_ = 0;
      s_.push(iter_pair(branch_.apex().children().begin(),
			branch_.apex().children().end()));
      n_ = &branch_.apex();

      //n_ = *(s_.top().first);
      //s_.top().first++;
    }

    template <typename T>
    void
    branch_iter<T>::next()
    {
      if (s_.size() == 0)
	invalidate();
      else
      {
	if (s_.top().first == s_.top().second)
	  //if (*(s_.top().first) == 0)
	{
	  deepness_ = futur_deepness_;
	  //deepness_--;
	  futur_deepness_ = deepness_;
// 	  std::cout << "dec :" << deepness_ << std::endl;
	  s_.pop();
	  next();
	  return;
	}
	else
	{
	  deepness_ = futur_deepness_;
// 	  std::cout << "st :" << deepness_ << std::endl;
	  n_ = *(s_.top().first);
	  s_.top().first++;

	  if (!n_)
	  {
// 	    std::cout << "browsing warning : nul pointer" << std::endl;
	    next();
	    return;
	  }

	  mln_assertion(n_);
	  if (n_->children().size() > 0)
	  {
	    futur_deepness_ = deepness_ + 1;
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


#endif // !MLN_UTIL_BRANCH_HH
