// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CONVERT_IMPL_FROM_VALUE_TO_VALUE_HH
# define MLN_CONVERT_IMPL_FROM_VALUE_TO_VALUE_HH

/// \file mln/convert/from_to.hh
///
/// General conversion procedure from a value to a value.
///
/// \todo Augment code + add checks.


# include <utility>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>
# include <mln/value/concept/all.hh>

# include <mln/core/site_set/p_run.hh>
# include <mln/metal/converts_to.hh>

# include <mln/convert/from_to.hxx>

namespace mln
{

  namespace convert
  {

    namespace impl
    {

      /// Conversion of a value \p from towards a value \p to.
      template <typename V, typename W>
      void
      from_value_to_value(const Value<V>& from, Value<W>& to);

      /// Specialization.
      template <typename V>
      void
      from_value_to_value(const Value<V>& from, Value<V>& to);



# ifndef MLN_INCLUDE_ONLY

      // Case 1:

      template <typename V, typename W>
      void
      from_value_to_value_(const mln::value::Vectorial<V>& from,
			         mln::value::Vectorial<W>&  to)
      {
	exact(to) = exact(from).to_equiv();
      }

      // Case 2:

      template <typename V, typename W>
      void
      from_value_to_value_(const mln::value::Scalar<V>& from,
			         mln::value::Scalar<W>&  to)
      {
	exact(to) = exact(from).to_equiv();
      }

      template <typename V, typename W>
      inline
      void
      from_value_to_value_(const Value<V>& from, Value<W>& to)
      {
	// No concept based conversion. Trying to find more specific
	// conversion with other from_to overloads.
	convert::over_load::from_to_(exact(from), exact(to));
      }


      // Facades.

      template <typename V, typename W>
      inline
      void
      from_value_to_value(const Value<V>& from, Value<W>& to)
      {
	from_value_to_value_(exact(from), exact(to));
      }

      template <typename V>
      inline
      void
      from_value_to_value(const Value<V>& from, Value<V>& to)
      {
	exact(to) = exact(from);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::convert::impl

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_IMPL_FROM_VALUE_TO_VALUE_HH
