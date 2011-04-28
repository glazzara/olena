// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_CONVERT_IMPL_FROM_FLOAT_TO_VALUE_HH
# define MLN_CONVERT_IMPL_FROM_FLOAT_TO_VALUE_HH

/// \file
///
/// General conversion procedure from a float to a value.
///
/// \todo Augment code + add checks.

# include <utility>
# include <mln/value/concept/integer.hh>
# include <mln/value/concept/floating.hh>
# include <mln/core/concept/value.hh>
# include <mln/math/round.hh>




namespace mln
{

  namespace convert
  {

      /// Conversion of a float \p from towards a value \p to.
      template <typename V>
      void
      from_to(const float& from, Value<V>& to);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// Case 1:

	template <typename V>
	inline
	void
	from_float_to_value(const float&	    from,
			    mln::value::Integer<V>& to)
	{
	  exact(to) = math::round<V>()(from);
	}

	// Case 2:

	template <typename V>
	inline
	void
	from_float_to_value(const float&	      from,
			    mln::value::Floating<V>&  to)
	{
	  exact(to) = from;
	}


	// Default: no conversion defined.

	template <typename V>
	inline
	void
	from_float_to_value(const float&	      from,
			    Value<V>&		      to)
	{
	  (void) from;
	  (void) to;
	  mlc_abort(V)::check();
	}

      } // end of namespace mln::convert::impl


      namespace internal
      {

	template <typename V>
	inline
	void
	from_float_to_value_dispatch(const float& from, Value<V>& to)
	{
	  impl::from_float_to_value(from, exact(to));
	}

      } // end of namespace mln::convert::internal


      namespace over_load
      {

	// Facades.


	// float-> Value
	template <typename V>
	inline
	void
	from_to_(const float& from, Value<V>& to)
	{
	  internal::from_float_to_value_dispatch(from, to);
	}

	// float-> unsigned
	inline
	void
	from_to_(const float&	    from,
		 unsigned&	    to)
	{
	  mln_precondition(from >= 0);
	  to = math::round<unsigned>()(from);
	}

	// float-> int
	inline
	void
	from_to_(const float&	    from,
		 int&		    to)
	{
	  to = math::round<int>()(from);
	}

	// float-> short
	inline
	void
	from_to_(const float&	    from,
		 short&		    to)
	{
	  to = math::round<short>()(from);
	}


      } // end of namespace mln::convert::over_load

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_IMPL_FROM_FLOAT_TO_VALUE_HH
