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

#ifndef MLN_VALUE_INTERNAL_VALUE_LIKE_HH
# define MLN_VALUE_INTERNAL_VALUE_LIKE_HH

/// \file
///
/// Factor code for value classes defined over another type.

# include <mln/core/concept/value.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace value
  {

    namespace internal
    {

      /*!
	\internal
	\brief Base class for value classes defined over another
	type.

	Parameters are \c V the equivalent value type and
	\c E the exact value type.
      */
      template < typename V, // Equivalent.
		 typename C, // Encoding.
		 typename N, // Interoperation.
		 typename E >
      struct value_like_ // FIXME :Remove -> : public Value<E>
      {
	/// Equivalent associated type.
	typedef V equiv;

	/// Encoding associated type.
	typedef C enc;

	/// Interoperation associated type.
	typedef N interop;

	/// Explicit conversion towards equivalent type.
	V to_equiv() const;

	/// Explicit conversion towards encoding type.
	const C& to_enc() const;

	/// Explicit conversion towards interoperation type.
	N to_interop() const;


	// Handle to encoding value.
	C& handle_() { return v_; }

      protected:
	enc v_; /// The encoding value.
      };


      /// General definition of the "equal to" operator between
      /// value-like types.
      ///
      template <typename V, typename C, typename N, typename E>
      bool operator==(const value_like_<V,C,N,E>& lhs, const value_like_<V,C,N,E>& rhs);


      /// General definition of the "less than" operator
      /// between value-like types.
      ///
      template <typename V, typename C, typename N, typename E>
      bool operator<(const value_like_<V,C,N,E>& lhs, const value_like_<V,C,N,E>& rhs);


# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename C, typename N, typename E>
      inline
      V
      value_like_<V,C,N,E>::to_equiv() const
      {
	return v_;
      }

      template <typename V, typename C, typename N, typename E>
      inline
      const C&
      value_like_<V,C,N,E>::to_enc() const
      {
	return v_;
      }

      template <typename V, typename C, typename N, typename E>
      inline
      N
      value_like_<V,C,N,E>::to_interop() const
      {
	return static_cast<N>(mln::internal::force_exact<E>(*this));
      }

      template <typename V, typename C, typename N, typename E>
      inline
      bool operator==(const value_like_<V,C,N,E>& lhs,
		      const value_like_<V,C,N,E>& rhs)
      {
	return lhs.to_enc() == rhs.to_enc();
      }

      template <typename V, typename C, typename N, typename E>
      inline
      bool operator<(const value_like_<V,C,N,E>& lhs,
		     const value_like_<V,C,N,E>& rhs)
      {
	return lhs.to_interop() < rhs.to_interop(); // FIXME HERE: Why interop?
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::value::internal

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_INTERNAL_VALUE_LIKE_HH
