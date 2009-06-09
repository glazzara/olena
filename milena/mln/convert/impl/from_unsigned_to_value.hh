// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CONVERT_IMPL_FROM_UNSIGNED_TO_VALUE_HH
# define MLN_CONVERT_IMPL_FROM_UNSIGNED_TO_VALUE_HH

/// \file
///
/// General conversion procedure from an unsigned to a value.
///
/// \todo Augment code + add checks.

# include <utility>
# include <mln/value/concept/integer.hh>
# include <mln/core/concept/value.hh>
# include <mln/value/label.hh>
# include <mln/math/round.hh>




namespace mln
{

  namespace convert
  {

      /// Conversion of an unsigned \p from towards a value \p to.
      template <typename V>
      void
      from_to(const unsigned& from, Value<V>& to);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Case 1:

	template <typename V>
	inline
	void
	from_unsigned_to_value(const unsigned&		  from,
			       mln::value::Integer<V>&	  to)
	{
	  exact(to) = from;
	}

	template <unsigned n>
	inline
	void
	from_unsigned_to_value(const unsigned&		  from,
			       mln::value::label<n>&	  to)
	{
	  exact(to) = from;
	}

	// Default: no conversion defined.

	template <typename V>
	inline
	void
	from_unsigned_to_value(const unsigned&,
			       Value<V>&)
	{
	  mlc_abort(V)::check();
	}

      } // end of namespace mln::convert::impl


      namespace internal
      {

	template <typename V>
	inline
	void
	from_unsigned_to_value_dispatch(const unsigned& from, Value<V>& to)
	{
	  impl::from_unsigned_to_value(from, exact(to));
	}

      } // end of namespace mln::convert::unsignedernal


      namespace over_load
      {

	// Facades.
	// unsigned-> Value
	template <typename V>
	void
	from_to_(const unsigned& from, Value<V>& to)
	{
	  internal::from_unsigned_to_value_dispatch(from, to);
	}

      } // end of namespace mln::convert::over_load


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_IMPL_FROM_UNSIGNED_TO_VALUE_HH
