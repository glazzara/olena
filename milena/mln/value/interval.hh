// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_INTERVAL_HH
# define MLN_VALUE_INTERVAL_HH

/*! \file
 *
 * \brief Define an interval between two values.
 *
 * \todo Likewise, code value::not_equal(t), less_than, etc.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  namespace value
  {

    /// FIXME: Doc!

    template <typename T>
    struct interval_ : public Object< interval_<T> >
    {
      interval_(const T& from, const T& to);
      T from, to;

      template <typename U>
      operator interval_<U>() const;
    };
    

    template <typename T>
    interval_<T>
    interval(const T& from, const T& to);


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    interval_<T>::interval_(const T& from, const T& to)
      : from(from),
	to(to)
    {
      mln_precondition(from <= to);
    }

    template <typename T>
    template <typename U>
    inline
    interval_<T>::operator interval_<U>() const
    {
      mln_invariant(from <= to);
      interval_<U> tmp(from, to);
      mln_postcondition(tmp.from <= tmp.to);
      return tmp;
    }

    template <typename T>
    inline
    interval_<T>
    interval(const T& from, const T& to)
    {
      mln_precondition(from <= to);
      interval_<T> tmp(from, to);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INTERVAL_HH
