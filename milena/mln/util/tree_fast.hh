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

#ifndef MLN_UTIL_TREE_FAST_HH
# define MLN_UTIL_TREE_FAST_HH

# include <vector>
# include <iostream>

# include <mln/core/contract.hh>

/*!
 * \file   mln/util/tree_fast.hh
 *
 * \brief  Definition of a fast generic general fast tree.
 *
 */

namespace mln
{

  namespace util
  {

    template <typename T>
    struct tree_fast
    {
      tree_fast();
      tree_fast(T& elt);

      const unsigned size() const;
      bool has (T& elt) const;
      unsigned search (T& elt) const;
      bool is_root (unsigned i) const;
      unsigned add_child (unsigned i, T& elt);
      unsigned add_parent (T& elt);

      std::vector<T> data_;
      std::vector<unsigned> parent_;
      std::vector<std::vector<unsigned> > child_;
      unsigned root_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    tree_fast<T>::tree_fast()
    {
    }

    template <typename T>
    tree_fast<T>::tree_fast(T& elt)
    {
      std::vector<unsigned> v;
      data_.push_back(elt);
      parent_.push_back(0);
      child_.push_back(v);
      root_ = 0;
    }

    template <typename T>
    const unsigned
    tree_fast<T>::size() const
    {
      return (data_.size ());
    }


    template <typename T>
    bool
    tree_fast<T>::has (T& elt) const
    {
      for (unsigned i = 0; i < data_.size (); ++i)
	if (data_[i] == elt)
	  return true;

      return false;
    }

    template <typename T>
    unsigned
    tree_fast<T>::search (T& elt) const
    {
      for (unsigned i = 0; i < data_.size (); ++i)
	if (data_[i] == elt)
	  return i;
      std::cerr << "BUGG !!!!"
		<< std::endl;
      return (unsigned)(-1);
    }

    template <typename T>
    bool
    tree_fast<T>::is_root (unsigned i) const
    {
      return (root_ == i);
    }

    template <typename T>
    unsigned
    tree_fast<T>::add_child (unsigned i, T& elt)
    {
      mln_assertion (i < data_.size ());
      std::vector<unsigned> v;
      data_.push_back(elt);
      parent_.push_back(i);
      child_.push_back(v);
      child_[i].push_back(data_.size () - 1);
      return (data_.size () - 1);
    }

    template <typename T>
    unsigned
    tree_fast<T>::add_parent (T& elt)
    {
      data_.push_back(elt);
      parent_.push_back(data_.size () - 1);
      std::vector<unsigned> v;
      v.push_back (root_);
      child_.push_back(v);
      parent_[root_] = data_.size () - 1;
      root_ = data_.size () - 1;
      return (data_.size () - 1);
    }




# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util


} // end of namespace mln


#endif // !MLN_UTIL_TREE_FAST_HH
