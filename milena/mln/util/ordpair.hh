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

#ifndef MLN_UTIL_ORDPAIR_HH
# define MLN_UTIL_ORDPAIR_HH

/*! \file mln/util/ordpair.hh
 *
 * \brief Definition of an ordered pair.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  namespace util
  {

    /// Ordered pair structure s.a. this->first <= this->second;
    /// ordered pairs are partially ordered using lexicographical
    /// ordering.
    template <typename T>
    struct ordpair_ : public mln::Object< ordpair_<T> >
    {
      ordpair_(const T& t1, const T& t2);
      T first;
      T second;
    };


    template <typename T>
    bool operator==(const ordpair_<T>& lhs, const ordpair_<T>& rhs);

    template <typename T>
    bool operator< (const ordpair_<T>& lhs, const ordpair_<T>& rhs);

    template <typename T>
    bool operator<=(const ordpair_<T>& lhs, const ordpair_<T>& rhs);


    template <typename T>
    std::ostream& operator<<(std::ostream& ostr, const ordpair_<T>& op);


    /// Routine to construct a ordpair on the fly.
    template <typename T>
    ordpair_<T> ordpair(const T& t1, const T& t2);


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    ordpair_<T>::ordpair_(const T& t1, const T& t2)
    {
      if (t1 > t2)
	{
	  first  = t2;
	  second = t1;
	}
      else
	{
	  first  = t1;
	  second = t2;
	}
      mln_postcondition(first <= second);
    }

    template <typename T>
    inline
    bool operator==(const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <typename T>
    inline
    bool operator< (const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return
	lhs.first < rhs.first ||
	(lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <typename T>
    inline
    bool operator<=(const ordpair_<T>& lhs, const ordpair_<T>& rhs)
    {
      return
	lhs.first < rhs.first ||
	(lhs.first == rhs.first && lhs.second <= rhs.second);
    }


    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const ordpair_<T>& op)
    {
      return ostr << '(' << op.first << ',' << op.second << ')';
    }


    template <typename T>
    inline
    ordpair_<T>
    ordpair(const T& t1, const T& t2)
    {
      ordpair_<T> tmp(t1, t2);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_CORE_EXACT_HH
