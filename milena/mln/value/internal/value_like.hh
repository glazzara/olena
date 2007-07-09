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

#ifndef MLN_VALUE_INTERNAL_VALUE_LIKE_HH
# define MLN_VALUE_INTERNAL_VALUE_LIKE_HH

/*! \file mln/value/internal/value_like.hh
 *
 * \brief Factor code for value classes defined over another type.
 *
 * \internal
 */

# include <mln/core/concept/value.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {

      /*! \brief Base class for value classes defined over another
       *  type.
       *
       * \internal
       *
       * Parameters are \c V the equivalent value type and \c E the
       * exact value type.
       */
      template <typename V, typename E>
      struct value_like_ : public Value<E>
      {
	/// Encoding associated type.
	typedef V enc;

	/// Equivalent associated type.
	typedef V equiv;

	/// Convertion towards equivalent type.
	operator V() const;

	/// Assignment from equivalent type.
	E& operator=(const V& v);

      protected:
	enc v_; /// The actual value.
      };


      /*! \brief General definition of the "equal to" operator between
       * value-like types.
       *
       * \relates value_like_
       */
      template <typename V, typename E>
      bool operator==(const value_like_<V,E>& lhs, const value_like_<V,E>& rhs);


      /*! \brief General definition of the "less than" operator
       *  between value-like types.
       *
       * \relates value_like_
       */
      template <typename V, typename E>
      bool operator<(const value_like_<V,E>& lhs, const value_like_<V,E>& rhs);


# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename E>
      value_like_<V, E>::operator V() const
      {
	return v_;
      }

      template <typename V, typename E>
      E&
      value_like_<V, E>::operator=(const V& v)
      {
	v_ = v;
	return exact(*this);
      }

      template <typename V, typename E>
      bool operator==(const value_like_<V, E>& lhs,
		      const value_like_<V, E>& rhs)
      {
	return V(lhs) == V(rhs);
      }

      template <typename V, typename E>
      bool operator<(const value_like_<V, E>& lhs,
		     const value_like_<V, E>& rhs)
      {
	return V(lhs) < V(rhs);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INTERNAL_VALUE_LIKE_HH
