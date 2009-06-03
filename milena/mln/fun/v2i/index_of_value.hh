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

#ifndef MLN_FUN_V2I_INDEX_OF_VALUE_HH
# define MLN_FUN_V2I_INDEX_OF_VALUE_HH

/// \file mln/fun/v2i/index_of_value.hh
///
/// \brief File that define a function that gives an index per value.

# include <mln/core/concept/function.hh>
# include <mln/trait/value_.hh>


namespace mln
{

  namespace fun
  {

    namespace v2i
    {

      template <typename T>
      struct index_of_value : Function_v2v< index_of_value<T> >,
			      private metal::bool_<(mln_dim(T) == 1)>::check_t
      {
	typedef unsigned result;
	unsigned operator()(const T& v) const;
      };

      template <>
      struct index_of_value<bool> : Function_v2v< index_of_value<bool> >
      {
	typedef unsigned result;
	unsigned operator()(bool b) const;
      };

      template <typename T>
      unsigned
      meta_index_of_value(const T& v);


# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      unsigned
      index_of_value<T>::operator()(const T& v) const
      {
	return unsigned( int(v) - int(mln_min(T)) );
      }

      inline
      unsigned index_of_value<bool>::operator()(bool b) const
      {
	return b ? 1u : 0u;
      }

      template <typename T>
      inline
      unsigned
      meta_index_of_value(const T& v)
      {
	index_of_value<T> f;
	return f(v);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2i

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_V2I_INDEX_OF_VALUE_HH
