// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_VALUE_OTHER_HH
# define MLN_VALUE_OTHER_HH

/*! \file
 *
 * \brief Definition of the mln::value::other routine.
 */

# include <mln/core/concept/value.hh>
# include <mln/value/set.hh>
# include <mln/value/concept/all.hh>


namespace mln
{

  namespace value
  {


    /// Give an other value than \p val. 
    template <typename V>
    V other(const V& val);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename V>
      V
      other_(const void*, const V& val) // Surely a builtin.
      {
	return val + 1;
      }

      inline
      bool
      other_(const void*, const bool& val)
      {
	return ! val;
      }


      template <typename V>
      inline
      V
      other_(mln::trait::value::quant::high, const Value<V>& val_)
      {
	const V& val = exact(val_);
	return val.other();
      }

      template <typename V>
      inline
      V
      other_(mln::trait::value::quant::high, const value::Scalar<V>& val_)
      {
	const V& val = exact(val_);
	return (mln_max(V) - val) + mln_min(V);
      }

      template <typename V>
      inline
      V
      other_(mln::trait::value::quant::low, const Value<V>& val_)
      {
	const V& val = exact(val_);
	value::set<V> s;
	unsigned i = s.index_of(val);
	mln_assertion(s.nvalues() >= 2);
	return i == 0 ? s[1] : s[0];
      }

      template <typename V>
      inline
      V
      other_(const Value<V>*, const V& val)
      {
	return other_(mln_trait_value_quant(V)(), val);
      }

    } // end of namespace mln::value::internal


    template <typename V>
    inline
    V other(const V& val)
    {
      V tmp = internal::other_(&val, val);
      mln_postcondition(tmp != val);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_OTHER_HH
