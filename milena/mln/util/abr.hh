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

#ifndef MLN_UTIL_ABR_HH
# define MLN_UTIL_ABR_HH

# include <vector>

/*!
 * \file   mln/util/abr.hh
 *
 * \brief  Definition of a generic general tree.
 *
 */

namespace mln
{

  namespace util
  {

    template <typename T>
    struct abr
    {
      abr();
      abr(T& elt);

      void add_child(T& elt);
      void print_rec(int n) const;
      void print(void) const;
      int  search_rec(abr<T>** res, T& elt);
      abr<T>* search(T& elt);

      T& elt_;
      abr<T>* parent_;
      std::vector< abr<T>* > child_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    abr<T>::abr()
      : elt_ (0),
	parent_ (0)
    {
    }

    template <typename T>
    abr<T>::abr(T& elt)
      : elt_ (elt),
	parent_ (0)
    {
    }

    template <typename T>
    void
    abr<T>::add_child(T& elt)
    {
      abr<T>* s = new abr<T>(elt);

      s->parent_ = this;
      this->child_.push_back(s);
    }

    template <typename T>
    int
    abr<T>::search_rec(abr<T>** res, T& elt)
    {
      if (elt == this->elt_)
	{
	  *res = this;
	  return 1;
	}
      else
	{
	  for (typename std::vector<abr<T>* >::iterator it = this->child_.begin();
	       it != this->child_.end(); ++it)
	    {
	      if ((**it).search_rec(res, elt))
		return 1;
	    }
	}
      return 0;
    }

    template <typename T>
    abr<T>*
    abr<T>::search(T& elt)
    {
      abr<T>* res = 0;

      if (search_rec(&res, elt))
	return res;
      return 0;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util


} // end of namespace mln


#endif // !MLN_UTIL_ABR_HH
