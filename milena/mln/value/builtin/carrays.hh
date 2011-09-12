// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_VALUE_BUILTIN_CARRAYS_HH
# define MLN_VALUE_BUILTIN_CARRAYS_HH

/// \file
///
/// Traits definition for C arrays.

# include <sstream>

# include <mln/value/concept/built_in.hh>
# include <mln/value/concept/vectorial.hh>
# include <mln/trait/value_.hh>


namespace mln
{


  template <unsigned n, typename T>
  struct category< T[n] >
  {
    typedef value::Built_In< value::Vectorial<void> > ret;
  };


  namespace trait
  {

    template <unsigned n, typename T>
    struct value_< T[n] >
    {
      enum {
	dim   = n,
	nbits = n * value_<T>::nbits,
	card  = n * value_<T>::card
      };
      typedef T comp;

      typedef value::nature::vectorial nature;
      typedef mln_trait_value_kind(T)  kind;
      typedef value::quant::high       quant;

      static const char* name()
      {
	std::ostringstream s;
	s << value_<T>::name() << '[' << n << ']';
	return s.str().c_str();
      }
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_CARRAYS_HH
